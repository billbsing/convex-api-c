/*
 *
 *
 * test_convex_get_account_balance
 *
 *
 */

#include <stdio.h>
#include <convex.h>
#include <assert.h>
#include <string.h>

int main() {

    int result;
    convex_p convex = convex_init("https://convex.world");
    assert(convex);

    amount_t balance;
    result = convex_get_account_balance(convex, 9, &balance);
    assert(result == CONVEX_OK);

    printf("balance: %ld\n", balance);
    assert(balance > 999999999);
    result = convex_close(convex);
    assert(result == CONVEX_OK);

}
