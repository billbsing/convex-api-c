/**
 *
 *
 * test_convex_create_account
 *
 *
 */

#include <assert.h>
#include "convex.h"

int main() {

    unsigned long address;
    int result;

    convex_p convex = convex_init("https://convex.world");
    assert(convex);
    convex_key_pair_p key_pair = convex_key_pair_init();
    assert(key_pair);

    result = convex_create_account(convex, key_pair, &address);
    assert(result == CONVEX_OK);
    assert(address > 20);

    result = convex_key_pair_close(key_pair);
    assert(result == CONVEX_OK);
    result = convex_close(convex);
    assert(result == CONVEX_OK);

    return 0;

}
