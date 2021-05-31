/*
 *
 *
 *  test_convex_account_sign_data
 *
 *
 */
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "convex_account.h"

int main() {

    const unsigned char HASH_DATA[] = {
        0x5b, 0xb1, 0xce, 0x71, 0x82, 0x41, 0xbf, 0xec, 0x11, 0x05, 0x52, 0xb8, 0x6b, 0xb7, 0xcc, 0xcf,
        0x0d, 0x95, 0xb8, 0xa5, 0xf4, 0x62, 0xfb, 0xf6, 0xdf, 0xf7, 0xc4, 0x85, 0x43, 0x62, 0x2b, 0xa5,
    };

    const unsigned char SIGN_DATA[] = {
        0x7e, 0xce, 0xff, 0xab, 0x47, 0x29, 0x5b, 0xe3, 0x89, 0x1e, 0xa7, 0x45, 0x83, 0x8a, 0x99, 0x10, 
        0x2b, 0xfa, 0xf5, 0x25, 0xec, 0x43, 0x63, 0x23, 0x66, 0xc7, 0xec, 0x3f, 0x54, 0xdb, 0x48, 0x22, 
        0xb5, 0xd5, 0x81, 0x57, 0x3a, 0xec, 0xde, 0x94, 0xc4, 0x20, 0x55, 0x4f, 0x96, 0x3b, 0xae, 0xbb, 
        0xf4, 0x12, 0xe4, 0x30, 0x4a, 0xd8, 0x63, 0x68, 0x86, 0xdd, 0xfa, 0x7b, 0x10, 0x49, 0xf7, 0x0e,
	};

    const char *PRIVATE_TEST_KEY_TEXT = "-----BEGIN ENCRYPTED PRIVATE KEY-----\n\
MIGbMFcGCSqGSIb3DQEFDTBKMCkGCSqGSIb3DQEFDDAcBAi3qm1zgjCO5gICCAAw\n\
DAYIKoZIhvcNAgkFADAdBglghkgBZQMEASoEENjvj1nzc0Qy22L+Zi+n7yIEQMLW\n\
o++Jzwlcg3PbW1Y2PxicdFHM3dBOgTWmGsvfZiLhSxTluXTNRCZ8ZLL5pi7JWtCl\n\
JAr4iFzPLkM18YEP2ZE=\n\
-----END ENCRYPTED PRIVATE KEY-----";

const char *TEST_PASSWORD = "secret";

    convex_account_p account;
    int result;

    // load in stardard test account
    account = convex_account_init_from_text(PRIVATE_TEST_KEY_TEXT, TEST_PASSWORD);
    assert(account);

    int signed_data_length = 64;
    unsigned char signed_data[signed_data_length];
    memset(signed_data, 0, signed_data_length);

    result = convex_account_sign_data(account, HASH_DATA, sizeof(HASH_DATA), signed_data, &signed_data_length);
    assert(result == CONVEX_OK);

    /*
    for (int index = 0; index < signed_data_length; index ++) {
        printf("%02x, ", signed_data[index]);
        if (index % 16 == 0){
            printf("\n");
        }
    }
    printf("%d \n", sizeof(SIGN_DATA));
    */

    assert(memcmp(signed_data, SIGN_DATA, sizeof(SIGN_DATA)) == 0);

    /*
    int sign_text_length = (64 * 2) + 1;
    char sign_text[sign_text_length];
    result = convex_utils_bytes_to_hex(signed_data, signed_data_length, sign_text, &sign_text_length);
    assert(result == CONVEX_OK);

    // printf("signed data: %s\n", sign_text);
    assert(strcmp(sign_text, SIGN_TEXT) == 0);
    */
    result = convex_account_close(account);
    assert(result == CONVEX_OK);

    return 0;
}
