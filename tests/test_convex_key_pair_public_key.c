
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include <convex_key_pair.h>

const char *PRIVATE_TEST_KEY_TEXT = "-----BEGIN ENCRYPTED PRIVATE KEY-----\n\
MIGbMFcGCSqGSIb3DQEFDTBKMCkGCSqGSIb3DQEFDDAcBAi3qm1zgjCO5gICCAAw\n\
DAYIKoZIhvcNAgkFADAdBglghkgBZQMEASoEENjvj1nzc0Qy22L+Zi+n7yIEQMLW\n\
o++Jzwlcg3PbW1Y2PxicdFHM3dBOgTWmGsvfZiLhSxTluXTNRCZ8ZLL5pi7JWtCl\n\
JAr4iFzPLkM18YEP2ZE=\n\
-----END ENCRYPTED PRIVATE KEY-----";

const char *PUBLIC_KEY_HEX = "5288Fec4153b702430771DFAC8AeD0B21CAFca4344daE0d47B97F0bf532b3306";

const char *TEST_PASSWORD = "secret";
int main() {
    convex_key_pair_p key_pair;
    int result;

    key_pair = convex_key_pair_init_from_text(PRIVATE_TEST_KEY_TEXT, TEST_PASSWORD);
    assert(key_pair);

    assert(strcmp(convex_key_pair_get_public_key(key_pair), PUBLIC_KEY_HEX) == 0);

    result = convex_key_pair_close(key_pair);
    assert(result == CONVEX_OK);

    return 0;
}

