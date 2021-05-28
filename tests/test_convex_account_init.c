
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include <convex_account.h>

int main() {
    convex_account_p account;
    int result;

    // test full init
    account = convex_account_init();
    assert(account);
    result = convex_account_close(account);
    assert(result == CONVEX_OK);


    return 0;
}

