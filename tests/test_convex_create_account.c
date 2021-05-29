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
    convex_account_p account = convex_account_init();
    assert(result == CONVEX_OK);

    result = convex_create_account(convex, account, &address);
    assert(result == CONVEX_OK);
    assert(address > 20);

    result = convex_account_close(account);
    assert(result == CONVEX_OK);
    result = convex_close(convex);
    assert(result == CONVEX_OK);

    return 0;

}
