/*
 *  si-82-seccertificate-ct.c
 *  Security
 *
 *  Copyright (c) 2014 Apple Inc. All Rights Reserved.
 *
 */

#include <CoreFoundation/CoreFoundation.h>
#include <Security/SecCertificatePriv.h>
#include <stdlib.h>
#include <unistd.h>
#include <utilities/SecCFRelease.h>

#include "shared_regressions.h"

static
unsigned char serverF_cert_der[] = {
    0x30, 0x82, 0x03, 0x73, 0x30, 0x82, 0x02, 0xdc, 0xa0, 0x03, 0x02, 0x01,
    0x02, 0x02, 0x01, 0x15, 0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86,
    0xf7, 0x0d, 0x01, 0x01, 0x05, 0x05, 0x00, 0x30, 0x52, 0x31, 0x0b, 0x30,
    0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x02, 0x55, 0x53, 0x31, 0x1a,
    0x30, 0x18, 0x06, 0x03, 0x55, 0x04, 0x0a, 0x0c, 0x11, 0x63, 0x6f, 0x72,
    0x65, 0x6f, 0x73, 0x2d, 0x63, 0x74, 0x2d, 0x74, 0x65, 0x73, 0x74, 0x20,
    0x43, 0x41, 0x31, 0x13, 0x30, 0x11, 0x06, 0x03, 0x55, 0x04, 0x08, 0x0c,
    0x0a, 0x43, 0x61, 0x6c, 0x69, 0x66, 0x6f, 0x72, 0x6e, 0x69, 0x61, 0x31,
    0x12, 0x30, 0x10, 0x06, 0x03, 0x55, 0x04, 0x07, 0x0c, 0x09, 0x43, 0x75,
    0x70, 0x65, 0x72, 0x74, 0x69, 0x6e, 0x6f, 0x30, 0x1e, 0x17, 0x0d, 0x31,
    0x32, 0x30, 0x36, 0x30, 0x31, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x5a,
    0x17, 0x0d, 0x32, 0x32, 0x30, 0x36, 0x30, 0x31, 0x30, 0x30, 0x30, 0x30,
    0x30, 0x30, 0x5a, 0x30, 0x72, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55,
    0x04, 0x06, 0x13, 0x02, 0x55, 0x53, 0x31, 0x17, 0x30, 0x15, 0x06, 0x03,
    0x55, 0x04, 0x0a, 0x0c, 0x0e, 0x63, 0x6f, 0x72, 0x65, 0x6f, 0x73, 0x2d,
    0x63, 0x74, 0x2d, 0x74, 0x65, 0x73, 0x74, 0x31, 0x13, 0x30, 0x11, 0x06,
    0x03, 0x55, 0x04, 0x08, 0x0c, 0x0a, 0x43, 0x61, 0x6c, 0x69, 0x66, 0x6f,
    0x72, 0x6e, 0x69, 0x61, 0x31, 0x12, 0x30, 0x10, 0x06, 0x03, 0x55, 0x04,
    0x07, 0x0c, 0x09, 0x43, 0x75, 0x70, 0x65, 0x72, 0x74, 0x69, 0x6e, 0x6f,
    0x31, 0x21, 0x30, 0x1f, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0c, 0x18, 0x63,
    0x6f, 0x72, 0x65, 0x6f, 0x73, 0x2d, 0x63, 0x74, 0x2d, 0x74, 0x65, 0x73,
    0x74, 0x2e, 0x61, 0x70, 0x70, 0x6c, 0x65, 0x2e, 0x63, 0x6f, 0x6d, 0x30,
    0x81, 0x9f, 0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d,
    0x01, 0x01, 0x01, 0x05, 0x00, 0x03, 0x81, 0x8d, 0x00, 0x30, 0x81, 0x89,
    0x02, 0x81, 0x81, 0x00, 0xd2, 0x49, 0x0c, 0xd0, 0xc5, 0xa8, 0xc3, 0x0f,
    0x36, 0x99, 0x54, 0x00, 0xd7, 0xf0, 0x2a, 0xcb, 0x21, 0x20, 0x4c, 0xac,
    0xaa, 0xcb, 0x36, 0x20, 0x72, 0x78, 0x05, 0xd1, 0xc2, 0xf9, 0xce, 0xc9,
    0x5b, 0xbc, 0x38, 0xda, 0xdd, 0x27, 0xf7, 0x6b, 0x0a, 0xf0, 0x16, 0xe2,
    0xc9, 0x74, 0x8c, 0x47, 0x5b, 0x07, 0x91, 0xa5, 0x6c, 0xcf, 0xf9, 0x0a,
    0x05, 0xb3, 0x05, 0x6a, 0xbe, 0x59, 0xdb, 0xa2, 0x1b, 0x21, 0x29, 0xe1,
    0xef, 0x0d, 0x4f, 0xa1, 0xc5, 0xbd, 0x16, 0xeb, 0x8c, 0x45, 0x6f, 0x64,
    0x42, 0x93, 0x82, 0xb3, 0x6d, 0xff, 0x83, 0x61, 0xdc, 0xcf, 0x8d, 0xd0,
    0x09, 0x2c, 0x37, 0x87, 0x1b, 0x75, 0xf6, 0xb3, 0xf8, 0x45, 0xef, 0xe2,
    0xcb, 0xff, 0x6d, 0xbb, 0xe4, 0xa5, 0x29, 0xee, 0xc0, 0x78, 0x17, 0x94,
    0xdc, 0x6b, 0xc7, 0x46, 0x01, 0x74, 0xf9, 0x65, 0x3b, 0x59, 0x21, 0xf5,
    0x02, 0x03, 0x01, 0x00, 0x01, 0xa3, 0x82, 0x01, 0x37, 0x30, 0x82, 0x01,
    0x33, 0x30, 0x1d, 0x06, 0x03, 0x55, 0x1d, 0x0e, 0x04, 0x16, 0x04, 0x14,
    0x69, 0x9d, 0x9f, 0x7e, 0xd9, 0x34, 0x7c, 0xfa, 0xd5, 0xc2, 0x7e, 0x02,
    0x0f, 0x1e, 0x4d, 0x1d, 0xa9, 0x8e, 0xa8, 0xcb, 0x30, 0x7a, 0x06, 0x03,
    0x55, 0x1d, 0x23, 0x04, 0x73, 0x30, 0x71, 0x80, 0x14, 0xdc, 0x16, 0x44,
    0x15, 0x3e, 0x53, 0x27, 0xd8, 0x68, 0x66, 0x41, 0x40, 0x88, 0x90, 0xe4,
    0x4e, 0x0a, 0xda, 0x08, 0xa9, 0xa1, 0x56, 0xa4, 0x54, 0x30, 0x52, 0x31,
    0x0b, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x02, 0x55, 0x53,
    0x31, 0x1a, 0x30, 0x18, 0x06, 0x03, 0x55, 0x04, 0x0a, 0x0c, 0x11, 0x63,
    0x6f, 0x72, 0x65, 0x6f, 0x73, 0x2d, 0x63, 0x74, 0x2d, 0x74, 0x65, 0x73,
    0x74, 0x20, 0x43, 0x41, 0x31, 0x13, 0x30, 0x11, 0x06, 0x03, 0x55, 0x04,
    0x08, 0x0c, 0x0a, 0x43, 0x61, 0x6c, 0x69, 0x66, 0x6f, 0x72, 0x6e, 0x69,
    0x61, 0x31, 0x12, 0x30, 0x10, 0x06, 0x03, 0x55, 0x04, 0x07, 0x0c, 0x09,
    0x43, 0x75, 0x70, 0x65, 0x72, 0x74, 0x69, 0x6e, 0x6f, 0x82, 0x01, 0x01,
    0x30, 0x09, 0x06, 0x03, 0x55, 0x1d, 0x13, 0x04, 0x02, 0x30, 0x00, 0x30,
    0x81, 0x8a, 0x06, 0x0a, 0x2b, 0x06, 0x01, 0x04, 0x01, 0xd6, 0x79, 0x02,
    0x04, 0x02, 0x04, 0x7c, 0x04, 0x7a, 0x00, 0x78, 0x00, 0x76, 0x00, 0xab,
    0xa8, 0xb5, 0xb4, 0x7d, 0x00, 0x00, 0x1b, 0x46, 0x58, 0x28, 0xc4, 0x0a,
    0xc7, 0x0b, 0x03, 0xf6, 0x91, 0x70, 0xa3, 0x5f, 0xed, 0xc8, 0x74, 0x40,
    0x3c, 0xd0, 0x58, 0x1d, 0x3c, 0x8c, 0x16, 0x00, 0x00, 0x01, 0x47, 0xdc,
    0x04, 0xbc, 0x5a, 0x00, 0x00, 0x04, 0x03, 0x00, 0x47, 0x30, 0x45, 0x02,
    0x20, 0x5b, 0x3b, 0xe2, 0x6b, 0xa2, 0xda, 0x49, 0xb2, 0xa5, 0x55, 0x1d,
    0x2f, 0x4d, 0x21, 0x2e, 0x2d, 0xf7, 0x59, 0xb3, 0x22, 0x1d, 0x90, 0x38,
    0x88, 0x77, 0xad, 0x49, 0xca, 0x28, 0x1d, 0x4a, 0xa8, 0x02, 0x21, 0x00,
    0xb7, 0x08, 0x08, 0xfb, 0x6a, 0x06, 0x13, 0xaa, 0xe6, 0x4d, 0x69, 0x44,
    0xce, 0xc0, 0x17, 0x8f, 0x3e, 0x80, 0x30, 0xe2, 0xd0, 0xe1, 0x8b, 0xc0,
    0x34, 0x28, 0x8b, 0xd8, 0x85, 0xb5, 0x14, 0x97, 0x30, 0x0d, 0x06, 0x09,
    0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x05, 0x05, 0x00, 0x03,
    0x81, 0x81, 0x00, 0xae, 0x8c, 0x7f, 0x63, 0x9d, 0xdd, 0xee, 0x4f, 0xc4,
    0xc5, 0x7b, 0x20, 0xb5, 0xe8, 0x89, 0x3e, 0x2c, 0xfe, 0x36, 0x0e, 0x31,
    0x1a, 0x38, 0xd6, 0xb3, 0xfd, 0x37, 0xeb, 0x26, 0xd0, 0x27, 0xfa, 0x04,
    0x12, 0x9c, 0xe2, 0x20, 0xe1, 0x61, 0xbf, 0xee, 0x60, 0x45, 0x84, 0xa0,
    0xea, 0xce, 0x1f, 0xf7, 0x73, 0x31, 0xd4, 0xd7, 0x87, 0xe7, 0xd5, 0x9f,
    0xff, 0x8d, 0x14, 0x32, 0x22, 0x89, 0xf6, 0x31, 0x38, 0xef, 0x1c, 0x36,
    0x55, 0x0d, 0x5f, 0x0d, 0x99, 0x36, 0x58, 0x6a, 0xa3, 0xff, 0xf0, 0xc7,
    0xe0, 0x5e, 0x02, 0x20, 0x9f, 0x04, 0x0a, 0xa4, 0xba, 0x1a, 0x1c, 0xb2,
    0x43, 0x85, 0xc2, 0xcc, 0xd2, 0x95, 0x8f, 0x20, 0x11, 0x1d, 0xea, 0x9e,
    0x10, 0xf1, 0x45, 0xd2, 0x4d, 0x95, 0x80, 0xed, 0xe1, 0x86, 0x71, 0xee,
    0x50, 0x0f, 0xb0, 0x73, 0x12, 0x32, 0xdd, 0x95, 0xc5, 0xb9, 0x54
};


__unused
static
unsigned char serverF_pre_cert_der[] = {
    0x30, 0x82, 0x02, 0xf9, 0x30, 0x82, 0x02, 0x62, 0xa0, 0x03, 0x02, 0x01,
    0x02, 0x02, 0x01, 0x15, 0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86,
    0xf7, 0x0d, 0x01, 0x01, 0x05, 0x05, 0x00, 0x30, 0x52, 0x31, 0x0b, 0x30,
    0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x02, 0x55, 0x53, 0x31, 0x1a,
    0x30, 0x18, 0x06, 0x03, 0x55, 0x04, 0x0a, 0x0c, 0x11, 0x63, 0x6f, 0x72,
    0x65, 0x6f, 0x73, 0x2d, 0x63, 0x74, 0x2d, 0x74, 0x65, 0x73, 0x74, 0x20,
    0x43, 0x41, 0x31, 0x13, 0x30, 0x11, 0x06, 0x03, 0x55, 0x04, 0x08, 0x0c,
    0x0a, 0x43, 0x61, 0x6c, 0x69, 0x66, 0x6f, 0x72, 0x6e, 0x69, 0x61, 0x31,
    0x12, 0x30, 0x10, 0x06, 0x03, 0x55, 0x04, 0x07, 0x0c, 0x09, 0x43, 0x75,
    0x70, 0x65, 0x72, 0x74, 0x69, 0x6e, 0x6f, 0x30, 0x1e, 0x17, 0x0d, 0x31,
    0x32, 0x30, 0x36, 0x30, 0x31, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x5a,
    0x17, 0x0d, 0x32, 0x32, 0x30, 0x36, 0x30, 0x31, 0x30, 0x30, 0x30, 0x30,
    0x30, 0x30, 0x5a, 0x30, 0x72, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55,
    0x04, 0x06, 0x13, 0x02, 0x55, 0x53, 0x31, 0x17, 0x30, 0x15, 0x06, 0x03,
    0x55, 0x04, 0x0a, 0x0c, 0x0e, 0x63, 0x6f, 0x72, 0x65, 0x6f, 0x73, 0x2d,
    0x63, 0x74, 0x2d, 0x74, 0x65, 0x73, 0x74, 0x31, 0x13, 0x30, 0x11, 0x06,
    0x03, 0x55, 0x04, 0x08, 0x0c, 0x0a, 0x43, 0x61, 0x6c, 0x69, 0x66, 0x6f,
    0x72, 0x6e, 0x69, 0x61, 0x31, 0x12, 0x30, 0x10, 0x06, 0x03, 0x55, 0x04,
    0x07, 0x0c, 0x09, 0x43, 0x75, 0x70, 0x65, 0x72, 0x74, 0x69, 0x6e, 0x6f,
    0x31, 0x21, 0x30, 0x1f, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0c, 0x18, 0x63,
    0x6f, 0x72, 0x65, 0x6f, 0x73, 0x2d, 0x63, 0x74, 0x2d, 0x74, 0x65, 0x73,
    0x74, 0x2e, 0x61, 0x70, 0x70, 0x6c, 0x65, 0x2e, 0x63, 0x6f, 0x6d, 0x30,
    0x81, 0x9f, 0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d,
    0x01, 0x01, 0x01, 0x05, 0x00, 0x03, 0x81, 0x8d, 0x00, 0x30, 0x81, 0x89,
    0x02, 0x81, 0x81, 0x00, 0xd2, 0x49, 0x0c, 0xd0, 0xc5, 0xa8, 0xc3, 0x0f,
    0x36, 0x99, 0x54, 0x00, 0xd7, 0xf0, 0x2a, 0xcb, 0x21, 0x20, 0x4c, 0xac,
    0xaa, 0xcb, 0x36, 0x20, 0x72, 0x78, 0x05, 0xd1, 0xc2, 0xf9, 0xce, 0xc9,
    0x5b, 0xbc, 0x38, 0xda, 0xdd, 0x27, 0xf7, 0x6b, 0x0a, 0xf0, 0x16, 0xe2,
    0xc9, 0x74, 0x8c, 0x47, 0x5b, 0x07, 0x91, 0xa5, 0x6c, 0xcf, 0xf9, 0x0a,
    0x05, 0xb3, 0x05, 0x6a, 0xbe, 0x59, 0xdb, 0xa2, 0x1b, 0x21, 0x29, 0xe1,
    0xef, 0x0d, 0x4f, 0xa1, 0xc5, 0xbd, 0x16, 0xeb, 0x8c, 0x45, 0x6f, 0x64,
    0x42, 0x93, 0x82, 0xb3, 0x6d, 0xff, 0x83, 0x61, 0xdc, 0xcf, 0x8d, 0xd0,
    0x09, 0x2c, 0x37, 0x87, 0x1b, 0x75, 0xf6, 0xb3, 0xf8, 0x45, 0xef, 0xe2,
    0xcb, 0xff, 0x6d, 0xbb, 0xe4, 0xa5, 0x29, 0xee, 0xc0, 0x78, 0x17, 0x94,
    0xdc, 0x6b, 0xc7, 0x46, 0x01, 0x74, 0xf9, 0x65, 0x3b, 0x59, 0x21, 0xf5,
    0x02, 0x03, 0x01, 0x00, 0x01, 0xa3, 0x81, 0xbe, 0x30, 0x81, 0xbb, 0x30,
    0x1d, 0x06, 0x03, 0x55, 0x1d, 0x0e, 0x04, 0x16, 0x04, 0x14, 0x69, 0x9d,
    0x9f, 0x7e, 0xd9, 0x34, 0x7c, 0xfa, 0xd5, 0xc2, 0x7e, 0x02, 0x0f, 0x1e,
    0x4d, 0x1d, 0xa9, 0x8e, 0xa8, 0xcb, 0x30, 0x7a, 0x06, 0x03, 0x55, 0x1d,
    0x23, 0x04, 0x73, 0x30, 0x71, 0x80, 0x14, 0xdc, 0x16, 0x44, 0x15, 0x3e,
    0x53, 0x27, 0xd8, 0x68, 0x66, 0x41, 0x40, 0x88, 0x90, 0xe4, 0x4e, 0x0a,
    0xda, 0x08, 0xa9, 0xa1, 0x56, 0xa4, 0x54, 0x30, 0x52, 0x31, 0x0b, 0x30,
    0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x02, 0x55, 0x53, 0x31, 0x1a,
    0x30, 0x18, 0x06, 0x03, 0x55, 0x04, 0x0a, 0x0c, 0x11, 0x63, 0x6f, 0x72,
    0x65, 0x6f, 0x73, 0x2d, 0x63, 0x74, 0x2d, 0x74, 0x65, 0x73, 0x74, 0x20,
    0x43, 0x41, 0x31, 0x13, 0x30, 0x11, 0x06, 0x03, 0x55, 0x04, 0x08, 0x0c,
    0x0a, 0x43, 0x61, 0x6c, 0x69, 0x66, 0x6f, 0x72, 0x6e, 0x69, 0x61, 0x31,
    0x12, 0x30, 0x10, 0x06, 0x03, 0x55, 0x04, 0x07, 0x0c, 0x09, 0x43, 0x75,
    0x70, 0x65, 0x72, 0x74, 0x69, 0x6e, 0x6f, 0x82, 0x01, 0x01, 0x30, 0x09,
    0x06, 0x03, 0x55, 0x1d, 0x13, 0x04, 0x02, 0x30, 0x00, 0x30, 0x13, 0x06,
    0x0a, 0x2b, 0x06, 0x01, 0x04, 0x01, 0xd6, 0x79, 0x02, 0x04, 0x03, 0x01,
    0x01, 0xff, 0x04, 0x02, 0x05, 0x00, 0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86,
    0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x05, 0x05, 0x00, 0x03, 0x81, 0x81,
    0x00, 0x66, 0x40, 0xd9, 0x60, 0xc6, 0x02, 0x39, 0x87, 0xc1, 0x15, 0x8e,
    0x3a, 0x3c, 0x4b, 0xf1, 0x29, 0xe7, 0x73, 0xbf, 0xca, 0xb4, 0x3c, 0x07,
    0x85, 0x9b, 0x8f, 0x2f, 0x23, 0xa9, 0x61, 0x58, 0x49, 0x04, 0xeb, 0xd5,
    0x6c, 0x4b, 0x2b, 0x4a, 0xd0, 0x73, 0xf1, 0xf2, 0x7a, 0x4a, 0x36, 0xd0,
    0x17, 0x1f, 0x61, 0xf8, 0xcf, 0xd7, 0xf4, 0x2c, 0x1a, 0x9d, 0x6a, 0xb0,
    0x5f, 0x3f, 0x06, 0xb5, 0xd0, 0x77, 0x18, 0x76, 0x69, 0x38, 0x3c, 0xc9,
    0x5e, 0xc7, 0x51, 0x12, 0xf2, 0x7a, 0xdf, 0xf7, 0x88, 0x70, 0x1f, 0xaf,
    0x3e, 0x50, 0xde, 0x95, 0x43, 0x1f, 0xee, 0x19, 0x80, 0xca, 0x7d, 0x7d,
    0x97, 0xce, 0x4f, 0x85, 0xbb, 0x7e, 0x5c, 0x67, 0xfe, 0x58, 0xb5, 0xf7,
    0xd2, 0x0c, 0x08, 0x64, 0xbe, 0x08, 0x49, 0x6d, 0xca, 0xbd, 0xd4, 0x54,
    0xa2, 0x8c, 0xeb, 0xe0, 0xa0, 0xa6, 0x0b, 0x39, 0x0c
};


__unused
static
unsigned char serverF_pre_cert_proof[] = {
    0x00, 0xab, 0xa8, 0xb5, 0xb4, 0x7d, 0x00, 0x00, 0x1b, 0x46, 0x58, 0x28,
    0xc4, 0x0a, 0xc7, 0x0b, 0x03, 0xf6, 0x91, 0x70, 0xa3, 0x5f, 0xed, 0xc8,
    0x74, 0x40, 0x3c, 0xd0, 0x58, 0x1d, 0x3c, 0x8c, 0x16, 0x00, 0x00, 0x01,
    0x47, 0xdc, 0x04, 0xbc, 0x5a, 0x00, 0x00, 0x04, 0x03, 0x00, 0x47, 0x30,
    0x45, 0x02, 0x20, 0x5b, 0x3b, 0xe2, 0x6b, 0xa2, 0xda, 0x49, 0xb2, 0xa5,
    0x55, 0x1d, 0x2f, 0x4d, 0x21, 0x2e, 0x2d, 0xf7, 0x59, 0xb3, 0x22, 0x1d,
    0x90, 0x38, 0x88, 0x77, 0xad, 0x49, 0xca, 0x28, 0x1d, 0x4a, 0xa8, 0x02,
    0x21, 0x00, 0xb7, 0x08, 0x08, 0xfb, 0x6a, 0x06, 0x13, 0xaa, 0xe6, 0x4d,
    0x69, 0x44, 0xce, 0xc0, 0x17, 0x8f, 0x3e, 0x80, 0x30, 0xe2, 0xd0, 0xe1,
    0x8b, 0xc0, 0x34, 0x28, 0x8b, 0xd8, 0x85, 0xb5, 0x14, 0x97
};


static void tests()
{
    SecCertificateRef certF = NULL;

    CFDataRef precertTBS = NULL;
    CFArrayRef proofs = NULL;
    CFDataRef spkiDigest = NULL;

    isnt(certF = SecCertificateCreateWithBytes(NULL, serverF_cert_der, sizeof(serverF_cert_der)), NULL, "create certF");

    isnt(precertTBS = SecCertificateCopyPrecertTBS(certF), NULL, "copy precertTBS");

    isnt(spkiDigest = SecCertificateCopySubjectPublicKeyInfoSHA256Digest(certF), NULL, "copy SPKI digest");

    isnt(proofs = SecCertificateCopySignedCertificateTimestamps(certF), NULL, "copy SCTs");

    CFReleaseSafe(certF);
    CFReleaseSafe(precertTBS);
    CFReleaseSafe(proofs);
    CFReleaseSafe(spkiDigest);

}

int si_82_seccertificate_ct(int argc, char *const *argv)
{
	plan_tests(4);

	tests();

	return 0;
}
