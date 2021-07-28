/**
 *
 *
 * test_convex_request_funds.c
 *
 *
 */

#include <assert.h>
#include "convex.h"

int main() {

    address_t address;
    amount_t amount = 1000;
    int result;
    amount_t result_amount = 0;

    convex_p convex = convex_init("https://convex.world");
    assert(convex);

    convex_key_pair_p key_pair = convex_key_pair_init();
    assert(result == CONVEX_OK);

    result = convex_create_account(convex, key_pair, &address);
    assert(result == CONVEX_OK);
    assert(address > 20);

    result = convex_request_funds(convex, amount, address, &result_amount);
    assert(result == CONVEX_OK);
    assert(result_amount == amount);

    result = convex_key_pair_close(key_pair);
    assert(result == CONVEX_OK);
    result = convex_close(convex);
    assert(result == CONVEX_OK);

    return 0;

}
