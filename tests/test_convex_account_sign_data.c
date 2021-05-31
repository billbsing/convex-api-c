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
    const char *SIGN_TEXT = "7eceffab47295be3891ea745838a99102bfaf525ec43632366c7ec3f54db4822b5d581573aecde94c420554f963baebbf412e4304ad8636886ddfa7b1049f70e";

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

    size_t signed_data_length = 64;
    unsigned char signed_data[signed_data_length];
    memset(signed_data, 0, signed_data_length);

    result = convex_account_sign_data(account, HASH_DATA, sizeof(HASH_DATA), signed_data, &signed_data_length);
    assert(result == CONVEX_OK);

    // printf("sign data length %d\n", signed_data_length);

    int sign_text_length = (64 * 2) + 1;
    char sign_text[sign_text_length];
    result = convex_utils_bytes_to_hex(signed_data, signed_data_length, sign_text, &sign_text_length);


    // printf("signed data: %s\n", sign_text);

    assert(strcmp(sign_text, SIGN_TEXT) == 0);
    result = convex_account_close(account);
    assert(result == CONVEX_OK);

    return 0;
}
