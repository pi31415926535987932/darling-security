/*
 * Copyright (c) 2005-2007,2009-2014 Apple Inc. All Rights Reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this
 * file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 *
 * testenv.c
 */

#include <fcntl.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>

#include "testmore.h"
#include "testenv.h"

#include <utilities/debugging.h>
#include <utilities/SecCFRelease.h>
#include <utilities/SecCFWrappers.h>
#include <utilities/SecFileLocations.h>

#include <dispatch/dispatch.h>


int test_strict_bats = 1;
int test_verbose = 0;
int test_onebatstest = 0;

#ifdef NO_SERVER
#include <securityd/spi.h>

static int current_dir = -1;
static char scratch_dir[50];
static char *home_var;
static bool keep_scratch_dir = false;

static int
rmdir_recursive(const char *path)
{
#if (!TARGET_IPHONE_SIMULATOR)
    char command_buf[256];
	if (strlen(path) + 10 > sizeof(command_buf) || strchr(path, '\''))
	{
		fprintf(stdout, "# rmdir_recursive: invalid path: %s", path);
		return -1;
	}

	sprintf(command_buf, "/bin/rm -rf '%s'", path);
	return system(command_buf);
#else
    fprintf(stdout, "# rmdir_recursive: simulator can't rmdir, leaving path: %s\n", path);
    return 0;
#endif
}
#endif

static int tests_init(void) {
    int ok = 0;
#ifdef NO_SERVER
	char preferences_dir[80];
	char library_dir[70];

	setup("tests_init");

    /* Create scratch dir for tests to run in. */
    sprintf(scratch_dir, "/tmp/tst-%d", getpid());
    if (keep_scratch_dir) {
        printf("running tests with HOME=%s\n", scratch_dir);
    }

    sprintf(library_dir, "%s/Library", scratch_dir);
    sprintf(preferences_dir, "%s/Preferences", library_dir);
    ok =  (ok_unix(mkdir(scratch_dir, 0755), "mkdir") &&
           ok_unix(current_dir = open(".", O_RDONLY), "open") &&
           ok_unix(chdir(scratch_dir), "chdir") &&
           ok_unix(setenv("HOME", scratch_dir, 1), "setenv") &&
           /* @@@ Work around a bug that the prefs code in
            libsecurity_keychain never creates the Library/Preferences
            dir. */
           ok_unix(mkdir(library_dir, 0755), "mkdir") &&
           ok_unix(mkdir(preferences_dir, 0755), "mkdir") &&
           ok(home_var = getenv("HOME"), "getenv"));
    
    if (ok > 0)
        securityd_init(scratch_dir);
#endif

    return ok;
}

static int
tests_end(void)
{
#ifdef NO_SERVER
	setup("tests_end");
	/* Restore previous cwd and remove scratch dir. */
	int ok = ok_unix(fchdir(current_dir), "fchdir");
	if (ok)
		ok = ok_unix(close(current_dir), "close");
	if (ok) {
		if (!keep_scratch_dir) {
			ok = ok_unix(rmdir_recursive(scratch_dir), "rmdir_recursive");
		}
	}
    
	return ok;
#else
    return 0;
#endif
}

static void usage(const char *progname)
{
    fprintf(stderr, "usage: %s [-k][-w][testname [testargs] ...]\n", progname);
    exit(1);
}

/* run one test, described by test, return info in test struct */
static int tests_run_test(struct one_test_s *test, int argc, char * const *argv)
{
    int ch;

    while ((ch = getopt(argc, argv, "v")) != -1)
    {
        switch  (ch)
        {
            case 'v':
                test_verbose++;
                break;
            default:
                usage(argv[0]);
        }
    }

    if (test_onebatstest)
        fprintf(stdout, "[TEST] %s\n", test->name);
    else
        fprintf(stdout, "[BEGIN] %s\n", test->name);

    const char *token = "PASS";
    if (test->entry == NULL) {
        fprintf(stdout, "%s:%d: error, no entry\n", __FILE__, __LINE__);
        test->failed_tests = 1;
    } else {
        struct timeval start, stop;
        gettimeofday(&start, NULL);
        test->entry(argc, argv);
        gettimeofday(&stop, NULL);
        /* this may overflow... */
        test->duration = (stop.tv_sec-start.tv_sec) * 1000 + (stop.tv_usec / 1000) - (start.tv_usec / 1000);
        if (test_plan_ok()) {
            token = "WARN";
        }
    }

    test_plan_final(&test->failed_tests, &test->todo_pass_tests, &test->todo_tests,
                    &test->actual_tests, &test->planned_tests,
                    &test->plan_file, &test->plan_line);
    if (test_verbose) {
        // TODO Use ccperf timing and  printing routines that use proper si scaling
        fprintf(stdout, "%s  took %lu ms\n", test->name, test->duration);
    }
    if (test->failed_tests) {
        token = "FAIL";
    }
    fprintf(stdout, "[%s] %s\n", token, test->name);

    return test->failed_tests;
}

static int strcmp_under_is_dash(const char *s, const char *t) {
    for (;;) {
        char a = *s++, b = *t++;
        if (a != b) {
            if (a != '_' || b != '-')
                return a - b;
        } else if (a == 0) {
            return 0;
        }
    }
}

static int tests_named_index(const char *testcase)
{
    int i;

    for (i = 0; testlist[i].name; ++i) {
        if (strcmp_under_is_dash(testlist[i].name, testcase) == 0) {
            return i;
        }
    }

    return -1;
}

static int tests_run_all(int argc, char * const *argv)
{
    int curroptind = optind;
    int i;
    int failcount=0;

    for (i = 0; testlist[i].name; ++i) {
        if(!testlist[i].off) {
            failcount+=tests_run_test(&testlist[i], argc, argv);
            optind = curroptind;
        }
    }

    return failcount;
}

static void
tests_summary(const char *progname) {
    int failed_tests = 0;
    int todo_pass_tests = 0;
    int todo_tests = 0;
    int actual_tests = 0;
    int planned_tests = 0;

    // First compute the totals to help decide if we need to print headers or not.
    for (int i = 0; testlist[i].name; ++i) {
        if (!testlist[i].off) {
            failed_tests += testlist[i].failed_tests;
            todo_pass_tests += testlist[i].todo_pass_tests;
            todo_tests += testlist[i].todo_tests;
            actual_tests += testlist[i].actual_tests;
            planned_tests += testlist[i].planned_tests;
        }
    }

    if (!test_onebatstest) {
        fprintf(stdout, "[%s] %s\n", failed_tests ? "FAIL" : (actual_tests == planned_tests ? "PASS" : "WARN"), progname);
    }

    fprintf(stdout, "[SUMMARY]\n");

    // -v -v makes the summary verbose as well.
    fprintf(stdout, "Test name                                                 failed !failed  todo  total   plan\n");
    if (test_verbose > 1 || failed_tests || todo_pass_tests || actual_tests != planned_tests || (test_verbose && todo_tests)) {
        fprintf(stdout, "============================================================================================\n");
    }
    for (int i = 0; testlist[i].name; ++i) {
        if (!testlist[i].off) {
            const char *token = NULL;
            if (testlist[i].failed_tests) {
                token = "FAIL";
            } else if (testlist[i].actual_tests != testlist[i].planned_tests) {
                token = "WARN";
            } else if (testlist[i].todo_pass_tests) {
                token = "WARN";
            } else if (test_verbose > 1 || (test_verbose && testlist[i].todo_tests)) {
                token = "PASS";
            }
            if (token) {
                fprintf(stdout, "[%s] %-50s %6d %6d %6d %6d %6d\n", token, testlist[i].name, testlist[i].failed_tests, testlist[i].todo_pass_tests, testlist[i].todo_tests, testlist[i].actual_tests, testlist[i].planned_tests);
            }
        }
    }
    if (test_verbose > 1 || failed_tests || todo_pass_tests || actual_tests != planned_tests || (test_verbose && todo_tests)) {
        fprintf(stdout, "============================================================================================\n");
    }
    fprintf(stdout, "Totals                                                    %6d %6d %6d %6d %6d\n", failed_tests, todo_pass_tests, todo_tests, actual_tests, planned_tests);
    
}

#define ASYNC_LOGGING  0
#define DATE_LOGGING   0

int
tests_begin(int argc, char * const *argv) {
    const char *testcase = NULL;
    bool initialized = false;
    __block bool print_security_logs = false;
    int testix = -1;
    int failcount = 0;
	int ch;
    int loop = 0;
    int list = 0;

#if ASYNC_LOGGING
    dispatch_queue_t show_queue = dispatch_queue_create("sec log queue", DISPATCH_QUEUE_SERIAL);
#endif

    security_log_handler handle_logs = ^(int level, CFStringRef scope, const char *function, const char *file, int line, CFStringRef message) {
        time_t now = time(NULL);
#if DATE_LOGGING
        char *date = ctime(&now);
        date[19] = '\0';
        CFStringRef logStr = CFStringCreateWithFormat(kCFAllocatorDefault, NULL,
                                                      CFSTR("%s %@{} %s %@\n"), date + 4,
                                                      scope ? scope : CFSTR(""), function, message);
#else
        CFStringRef logStr = CFStringCreateWithFormat(kCFAllocatorDefault, NULL,
                                                      CFSTR("%lu %@{} %s %@\n"), now,
                                                      scope ? scope : CFSTR(""), function, message);
#endif
#if ASYNC_LOGGING
        dispatch_async(show_queue, ^{ CFShow(logStr); CFReleaseSafe(logStr); });
#else
        CFShow(logStr);
        CFReleaseSafe(logStr);
#endif
    };

    // TODO Currently our callers do this, but we can move this here together with the build date info.
    const char *progname = strrchr(argv[0], '/');
    progname = progname ? progname + 1 : argv[0];

    for (;;) {
        while (!testcase && (ch = getopt(argc, argv, "bklL1vwqs")) != -1)
        {
            switch  (ch)
            {
#ifdef NO_SERVER
            case 'k':
                keep_scratch_dir = true;
                break;
#endif
            case 's':
                if (!print_security_logs) {
                    add_security_log_handler(handle_logs);
                    print_security_logs = true;
                }
                break;

            case 'b':
                test_strict_bats = 0;
                break;

            case 'v':
                test_verbose++;
                break;

            case 'w':
                sleep(100);
                break;
            case 'l':
                loop=1;
                break;
            case 'L':
                list=1;
                break;
            case '1':
                test_onebatstest=1;
                break;
            case '?':
            default:
                printf("invalid option %c\n",ch); 
                usage(argv[0]);
            }
        }

        if (optind < argc) {
            testix = tests_named_index(argv[optind]);
            if(testix<0) {
                printf("invalid test %s\n",argv[optind]); 
                usage(argv[0]);
            }
        }

        if (!list && !initialized && !test_onebatstest)
            fprintf(stdout, "[TEST] %s\n", progname);
        if (testix < 0) {
            if (!initialized) {
                tests_init();
                initialized = true;
                (void)initialized;
                if (!list)
                    failcount+=tests_run_all(argc, argv);
            }
            break;
        } else {
            if (!initialized) {
                tests_init();
                initialized = true;
                for (int i = 0; testlist[i].name; ++i) {
                    testlist[i].off = 1;
                }
            }
            optind++;
            testlist[testix].off = 0;
            if (!list)
                failcount+=tests_run_test(&testlist[testix], argc, argv);
            testix = -1;
        }
    }
    if (list) {
        for (int i = 0; testlist[i].name; ++i) {
            if (!testlist[i].off) {
                    fprintf(stdout, "%s\n", testlist[i].name);
            }
        }
    } else {
        tests_summary(progname);
    }
    
    remove_security_log_handler(handle_logs);
    fflush(stdout);


    /* Cleanups */
    tests_end();

    if (loop) {
        printf("Looping until key press 'q'. You can run leaks now.\n");
        while(getchar()!='q');
    }

    return failcount;
}

