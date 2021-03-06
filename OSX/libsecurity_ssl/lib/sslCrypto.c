/*
 * Copyright (c) 2006-2014 Apple Inc. All Rights Reserved.
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
 */

/*
 * sslCrypto.c - interface between SSL and crypto libraries
 */

#include "sslCrypto.h"
#include "sslContext.h"
#include "sslMemory.h"
#include "sslUtils.h"
#include "sslDebug.h"

#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include <Security/SecTrustPriv.h>
#include <Security/SecPolicy.h>
#include <Security/SecCertificate.h>

#include <AssertMacros.h>
#include "utilities/SecCFRelease.h"

#if TARGET_OS_IPHONE
#include <Security/SecRSAKey.h>
#include <Security/SecECKey.h>
#endif

/*
 * Get algorithm id for a SSLPubKey object.
 */
CFIndex sslPubKeyGetAlgorithmID(SecKeyRef pubKey)
{
#if TARGET_OS_IPHONE
	return SecKeyGetAlgorithmID(pubKey);
#else
	return SecKeyGetAlgorithmId(pubKey);
#endif
}

/*
 * Get algorithm id for a SSLPrivKey object.
 */
CFIndex sslPrivKeyGetAlgorithmID(SecKeyRef privKey)
{
#if TARGET_OS_IPHONE
	return SecKeyGetAlgorithmID(privKey);
#else
	return SecKeyGetAlgorithmId(privKey);
#endif
}

static
OSStatus sslCreateCFArrayFromList(const tls_buffer_list_t *list, CFArrayRef *cfArray)
{
    int err;
    CFMutableArrayRef array = NULL;
    CFDataRef data = NULL;

    err = errSSLInternal;

    array = CFArrayCreateMutable(kCFAllocatorDefault, 0, &kCFTypeArrayCallBacks);
    require(array, out);

    while(list) {
        require((data = CFDataCreate(kCFAllocatorDefault, list->buffer.data, list->buffer.length)), out);
        CFArrayAppendValue(array, data);
        CFReleaseNull(data);
        list=list->next;
    }

    *cfArray = array;
    return errSecSuccess;

out:
    CFReleaseSafe(data);
    CFReleaseSafe(array);
    return err;
}

OSStatus
sslCreateSecTrust(
	SSLContext				*ctx,
	CFArrayRef				certChain,
    SecTrustRef             *pTrust)	/* RETURNED */
{
	OSStatus status = errSecAllocate;
	CFStringRef peerDomainName = NULL;
	CFTypeRef policies = NULL;
	SecTrustRef trust = NULL;
    const char *peerDomainNameData = NULL;
    size_t peerDomainNameLen = 0;

    if(ctx->protocolSide==kSSLClientSide) {
        tls_handshake_get_peer_hostname(ctx->hdsk, &peerDomainNameData, &peerDomainNameLen);
    }

    if (CFArrayGetCount(certChain) == 0) {
		status = errSSLBadCert;
		goto errOut;
	}

    if (peerDomainNameLen && peerDomainNameData) {
        CFIndex len = peerDomainNameLen;
        if (peerDomainNameData[len - 1] == 0) {
            len--;
            //secwarning("peerDomainName is zero terminated!");
        }
        /* @@@ Double check that this is the correct encoding. */
        require(peerDomainName = CFStringCreateWithBytes(kCFAllocatorDefault,
            (const UInt8 *)peerDomainNameData, len,
            kCFStringEncodingUTF8, false), errOut);
    }

    /* If we are the client, our peer certificates must satisfy the
       ssl server policy. */
    bool use_server_policy = (ctx->protocolSide == kSSLClientSide);
	require(policies = SecPolicyCreateSSL(use_server_policy, peerDomainName), errOut);

	require_noerr(status = SecTrustCreateWithCertificates(certChain, policies,
		&trust), errOut);

    /* If we are the client, let's see if we have OCSP responses and SCTs in the TLS handshake */
    if(ctx->protocolSide == kSSLClientSide) {
        const tls_buffer_list_t *sct_list = tls_handshake_get_peer_sct_list(ctx->hdsk);
        const tls_buffer *ocsp_response = tls_handshake_get_peer_ocsp_response(ctx->hdsk);

        if(ocsp_response) {
            CFDataRef responseData = CFDataCreate(kCFAllocatorDefault, ocsp_response->data, ocsp_response->length);
            status = SecTrustSetOCSPResponse(trust, responseData);
            CFReleaseSafe(responseData);
            require_noerr(status, errOut);
        }

        if(sct_list) {
            CFArrayRef sctArray = NULL;
            require_noerr(status = sslCreateCFArrayFromList(sct_list, &sctArray), errOut);
#if TARGET_OS_IPHONE
            status = SecTrustSetSignedCertificateTimestamps(trust, sctArray);
#else
            status = noErr;
#endif
            CFReleaseSafe(sctArray);
            require_noerr(status, errOut);
        }
    }

	/* If we have trustedAnchors we set them here. */
    if (ctx->trustedCerts) {
        require_noerr(status = SecTrustSetAnchorCertificates(trust,
            ctx->trustedCerts), errOut);
        require_noerr(status = SecTrustSetAnchorCertificatesOnly(trust,
            ctx->trustedCertsOnly), errOut);
    }

    status = errSecSuccess;

errOut:
	CFReleaseSafe(peerDomainName);
	CFReleaseSafe(policies);

	*pTrust = trust;

	return status;
}

/* Return the first certificate reference from the supplied array
 * whose data matches the given certificate, or NULL if none match.
 */
static
SecCertificateRef
sslGetMatchingCertInArray(
	SecCertificateRef	certRef,
	CFArrayRef			certArray)
{
	SecCertificateRef matchedCert = NULL;

	if (certRef == NULL || certArray == NULL) {
		return NULL;
	}

	CFDataRef certData = SecCertificateCopyData(certRef);
	if (certData) {
		CFIndex idx, count = CFArrayGetCount(certArray);
		for(idx=0; idx<count; idx++) {
			SecCertificateRef aCert = (SecCertificateRef)CFArrayGetValueAtIndex(certArray, idx);
			CFDataRef aData = SecCertificateCopyData(aCert);
			if (aData && CFEqual(aData, certData)) {
				matchedCert = aCert;
			}
			CFReleaseSafe(aData);
			if (matchedCert)
				break;
		}
		CFReleaseSafe(certData);
	}

    return matchedCert;
}

/*
 * Verify a chain of DER-encoded certs.
 */
static OSStatus sslVerifyCertChain(
	SSLContext				*ctx,
	CFArrayRef				certChain)
{
	OSStatus status;
	SecTrustRef trust = NULL;

    /* renegotiate - start with a new SecTrustRef */
    CFReleaseNull(ctx->peerSecTrust);

    if(certChain==NULL) {
        if(ctx->protocolSide == kSSLClientSide) {
            /* No cert chain is always a trust failure on the server side */
            status = errSSLXCertChainInvalid;
            sslErrorLog("***Error: NULL server cert chain\n");
        } else {
            /* No cert chain on the client side is ok unless using kAlwaysAuthenticate */
            if(ctx->clientAuth == kAlwaysAuthenticate) {
                sslErrorLog("***Error: NULL client cert chain\n");
                status = errSSLXCertChainInvalid;
            } else {
                status = noErr;
            }
        }
        goto errOut;
    }

	status = sslCreateSecTrust(ctx, certChain, &trust);

	if (!ctx->enableCertVerify) {
		/* trivial case, this is caller's responsibility */
		status = errSecSuccess;
		goto errOut;
	}

	SecTrustResultType secTrustResult;
	require_noerr(status = SecTrustEvaluate(trust, &secTrustResult), errOut);
	switch (secTrustResult) {
        case kSecTrustResultUnspecified:
            /* cert chain valid, no special UserTrust assignments */
        case kSecTrustResultProceed:
            /* cert chain valid AND user explicitly trusts this */
            status = errSecSuccess;
            break;
        case kSecTrustResultDeny:
        case kSecTrustResultConfirm:
        case kSecTrustResultRecoverableTrustFailure:
        default:
            if(ctx->allowAnyRoot) {
                sslErrorLog("***Warning: accepting unverified cert chain\n");
                status = errSecSuccess;
            }
            else {
				/*
				 * If the caller provided a list of trusted leaf certs, check them here
				 */
				if(ctx->trustedLeafCerts) {
					if (sslGetMatchingCertInArray((SecCertificateRef)CFArrayGetValueAtIndex(certChain, 0),
								ctx->trustedLeafCerts)) {
						status = errSecSuccess;
						goto errOut;
					}
				}
				status = errSSLXCertChainInvalid;
            }
            /* Do we really need to return things like:
                   errSSLNoRootCert
                   errSSLUnknownRootCert
                   errSSLCertExpired
                   errSSLCertNotYetValid
                   errSSLHostNameMismatch
               for our client to see what went wrong, or should we just always
               return
                   errSSLXCertChainInvalid
               when something is wrong? */
            break;
	}

errOut:
	ctx->peerSecTrust = trust;

	return status;
}

/* Extract public SecKeyRef from Certificate Chain */
static
int sslCopyPeerPubKey(const SSLCertificate *certchain,
                      SecKeyRef            *pubKey)
{
    int err;
    check(pubKey);
    SecTrustRef trust = NULL;
    const SSLCertificate *cert;
    CFMutableArrayRef certArray = NULL;
    CFDataRef certData = NULL;
    SecCertificateRef cfCert = NULL;

    err = errSSLInternal;

    certArray = CFArrayCreateMutable(kCFAllocatorDefault, 0, &kCFTypeArrayCallBacks);
    cert = certchain;
    while(cert) {
        require((certData = CFDataCreate(kCFAllocatorDefault, cert->derCert.data, cert->derCert.length)), out);
        require_action((cfCert = SecCertificateCreateWithData(kCFAllocatorDefault, certData)), out, err=errSSLBadCert);
        CFArrayAppendValue(certArray, cfCert);
        CFReleaseNull(cfCert);
        CFReleaseNull(certData);
        cert=cert->next;
    }

    require_noerr((err=SecTrustCreateWithCertificates(certArray, NULL, &trust)), out);
    SecKeyRef key = SecTrustCopyPublicKey(trust);
    require_action(key, out, err=errSSLBadCert);

    *pubKey = key;

    err = errSecSuccess;

out:
    CFReleaseSafe(certData);
    CFReleaseSafe(cfCert);
    CFReleaseSafe(trust);
    CFReleaseSafe(certArray);

    return err;
}

/* Extract the pubkey from a cert chain, and send it to the tls_handshake context */
int tls_set_peer_pubkey(SSLContext *ctx)
{
    int err;
    CFIndex algId;
    SecKeyRef pubkey = NULL;
    CFDataRef modulus = NULL;
    CFDataRef exponent = NULL;
    CFDataRef ecpubdata = NULL;
    const SSLCertificate *certchain = NULL;

    certchain = tls_handshake_get_peer_certificates(ctx->hdsk);
    CFReleaseNull(ctx->peerCert);

    /* If there is no certchain, then we don't need to set the pubkey in coreTLS */
    /* We should really set it to "NULL" or none, but we need to fix the coreTLS API */
    /* See: <rdar://problem/19723662> coreTLS: replace tls_handshake_set_peer_rsa_public_key and tls_handshake_set_peer_ec_public_key with a common function */
    if(!certchain)
        return 0;

    ctx->peerCert = tls_get_peer_certs(certchain);

#if 0
    { /* dump certs */
        int i=0;
        int j;
        const SSLCertificate *tmp = certchain;
        while(tmp) {
            printf("cert%d[] = {", i);
            for(j=0; j<tmp->derCert.length; j++) {
                if((j&0xf)==0)
                    printf("\n");
                printf("0x%02x, ", tmp->derCert.data[j]);
            }
            printf("}\n");
            tmp=tmp->next;
            i++;
        }
    }
#endif

    require_noerr((err=sslCopyPeerPubKey(certchain, &pubkey)), errOut);

#if TARGET_OS_IPHONE
	algId = SecKeyGetAlgorithmID(pubkey);
#else
	algId = SecKeyGetAlgorithmId(pubkey);
#endif

    err = errSSLCrypto;

    switch(algId) {
        case kSecRSAAlgorithmID:
        {
            require((modulus = SecKeyCopyModulus(pubkey)), errOut);
            require((exponent = SecKeyCopyExponent(pubkey)), errOut);

            tls_buffer mod;
            tls_buffer exp;

            mod.data = (uint8_t *)CFDataGetBytePtr(modulus);
            mod.length = CFDataGetLength(modulus);

            exp.data = (uint8_t *)CFDataGetBytePtr(exponent);
            exp.length = CFDataGetLength(exponent);

            err = tls_handshake_set_peer_rsa_public_key(ctx->hdsk, &mod, &exp);
            break;
        }
        case kSecECDSAAlgorithmID:
        {
            tls_named_curve curve = SecECKeyGetNamedCurve(pubkey);
            require((ecpubdata = SecECKeyCopyPublicBits(pubkey)), errOut);

            tls_buffer pubdata;
            pubdata.data = (uint8_t *)CFDataGetBytePtr(ecpubdata);
            pubdata.length = CFDataGetLength(ecpubdata);

            err = tls_handshake_set_peer_ec_public_key(ctx->hdsk, curve, &pubdata);

            break;
        }
        default:
            break;
    }

errOut:
    CFReleaseSafe(pubkey);
    CFReleaseSafe(modulus);
    CFReleaseSafe(exponent);
    CFReleaseSafe(ecpubdata);
    
    return err;
}

/* Convert cert in DER format into an CFArray of SecCertificateRef */
CFArrayRef
tls_get_peer_certs(const SSLCertificate *certs)
{
    const SSLCertificate *cert;

    CFMutableArrayRef certArray = NULL;
    CFDataRef certData = NULL;
    SecCertificateRef cfCert = NULL;

    certArray = CFArrayCreateMutable(kCFAllocatorDefault, 0, &kCFTypeArrayCallBacks);
    require(certArray, out);
    cert = certs;
    while(cert) {
        require((certData = CFDataCreate(kCFAllocatorDefault, cert->derCert.data, cert->derCert.length)), out);
        require((cfCert = SecCertificateCreateWithData(kCFAllocatorDefault, certData)), out);
        CFArrayAppendValue(certArray, cfCert);
        CFReleaseNull(cfCert);
        CFReleaseNull(certData);
        cert=cert->next;
    }

    return certArray;

out:
    CFReleaseNull(cfCert);
    CFReleaseNull(certData);
    CFReleaseNull(certArray);
    return NULL;
}

int
tls_verify_peer_cert(SSLContext *ctx)
{
    int err = 0;
    OSStatus st;

    /* Note: A verification failure here does not cause the function to return an error.
       This will allow the handshake to continue, coreTLS will eventually returns an error,
       after sending the appropriate alert messages, based on the trust value set with the
       call to tls_handshake_set_peer_trust(). In some case a verification failure here is 
       normal, for example if there is no cert (eg: PSK and Anon DH ciphersuites) */

    st = sslVerifyCertChain(ctx, ctx->peerCert);
    tls_handshake_trust_t trust;
    switch (st) {
        case errSecSuccess:
            trust = tls_handshake_trust_ok;
            break;
        case errSSLUnknownRootCert:
        case errSSLNoRootCert:
            trust = tls_handshake_trust_unknown_root;
            break;
        case errSSLCertExpired:
        case errSSLCertNotYetValid:
            trust = tls_handshake_trust_cert_expired;
            break;
        case errSSLXCertChainInvalid:
        default:
            trust = tls_handshake_trust_cert_invalid;
            break;
    }

    tls_handshake_set_peer_trust(ctx->hdsk, trust);

    /* Now that trust has been (possibly) evaluated,
       we check if we need to break out of the handshake */
    if(ctx->protocolSide == kSSLServerSide) {
        /*
         * Schedule return to the caller to verify the client's identity.
         * This will return even if there was no client cert sent.
         */
        if (ctx->breakOnClientAuth) {
            err = errSSLClientAuthCompleted;
        }
    } else if(ctx->peerCert) {
        /*
         * Schedule return to the caller to verify the server's identity.
         * This will only return if a server cert was sent. In other cases
         * such as PSK and AnonDH, we don't want to break out of the handshake.
         */
        if (ctx->breakOnServerAuth) {
            err = errSSLServerAuthCompleted;
        }
    }

    return err;
}

/*
 * After ciphersuite negotiation is complete, verify that we have
 * the capability of actually performing the selected cipher.
 * Currently we just verify that we have a cert and private signing
 * key, if needed, and that the signing key's algorithm matches the
 * expected key exchange method.
 *
 * This is currently called from FindCipherSpec(), after it sets
 * ctx->selectedCipherSpec to a (supposedly) valid value, and from
 * sslBuildCipherSpecArray(), in server mode (pre-negotiation) only.
 */

#if 0
OSStatus sslVerifySelectedCipher(SSLContext *ctx)
{

    if(ctx->protocolSide == kSSLClientSide) {
        return errSecSuccess;
    }
#if SSL_PAC_SERVER_ENABLE
    if((ctx->masterSecretCallback != NULL) &&
       (ctx->sessionTicket.data != NULL)) {
            /* EAP via PAC resumption; we can do it */
	return errSecSuccess;
    }
#endif	/* SSL_PAC_SERVER_ENABLE */

    CFIndex requireAlg;
    switch (ctx->selectedCipherSpecParams.keyExchangeMethod) {
        case SSL_RSA:
        case SSL_RSA_EXPORT:
	case SSL_DH_RSA:
	case SSL_DH_RSA_EXPORT:
	case SSL_DHE_RSA:
	case SSL_DHE_RSA_EXPORT:
            requireAlg = kSecRSAAlgorithmID;
            break;
 	case SSL_DHE_DSS:
	case SSL_DHE_DSS_EXPORT:
 	case SSL_DH_DSS:
	case SSL_DH_DSS_EXPORT:
            requireAlg = kSecDSAAlgorithmID;
            break;
	case SSL_DH_anon:
	case SSL_DH_anon_EXPORT:
        case TLS_PSK:
            requireAlg = kSecNullAlgorithmID; /* no signing key */
            break;
        /*
         * When SSL_ECDSA_SERVER is true and we support ECDSA on the server side,
         * we'll need to add some logic here...
         */
#if SSL_ECDSA_SERVER
        case SSL_ECDHE_ECDSA:
        case SSL_ECDHE_RSA:
        case SSL_ECDH_ECDSA:
        case SSL_ECDH_RSA:
        case SSL_ECDH_anon:
            requireAlg = kSecECDSAAlgorithmID;
            break;
#endif

	default:
            /* needs update per cipherSpecs.c */
            assert(0);
            sslErrorLog("sslVerifySelectedCipher: unknown key exchange method\n");
            return errSSLInternal;
    }

    if(requireAlg == kSecNullAlgorithmID) {
	return errSecSuccess;
    }

    /* private signing key required */
    if(ctx->signingPrivKeyRef == NULL) {
	sslErrorLog("sslVerifySelectedCipher: no signing key\n");
	return errSSLBadConfiguration;
    }

    /* Check the alg of our signing key. */
    CFIndex keyAlg = sslPrivKeyGetAlgorithmID(ctx->signingPrivKeyRef);
    if (requireAlg != keyAlg) {
	sslErrorLog("sslVerifySelectedCipher: signing key alg mismatch\n");
	return errSSLBadConfiguration;
    }

    return errSecSuccess;
}

#endif
