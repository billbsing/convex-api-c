/*
 * test_convex_transfer
 *
 */

#include <stdio.h>
#include <convex.h>
#include <assert.h>
#include <string.h>

int main() {
    convex_p convex;
    const char *url = "https://convex.world";
    int result;
    address_t address;
    address_t to_address = 9;
    amount_t amount = 10000000;
    amount_t result_amount;
    const char *transaction = "(map inc [1 2 3 4 5])";
    const char *transaction_result = "{\"value\":[2,3,4,5,6]}";

    convex = convex_init(url);
    assert(convex);

    convex_account_p account = convex_account_init();
    assert(account);

    result = convex_create_account(convex, account, &address);
    assert(result == CONVEX_OK);
    assert(address > 20);

    result = convex_request_funds(convex, amount, address, &result_amount);
    assert(result == CONVEX_OK);
    assert(result_amount == amount);

    result = convex_transfer(convex, to_address, 10000, account, address);
    assert(result == CONVEX_OK);

    result = convex_close(convex);
    assert(result == CONVEX_OK);

    return 0;
}

