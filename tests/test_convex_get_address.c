/**
 *
 *
 * test_convex_get_address.c
 *
 *
 */

#include <assert.h>
#include "convex.h"

int main() {

    address_t address;
    amount_t amount = 1000;
    int result;

    convex_p convex = convex_init("https://convex.world");
    assert(convex);

    result = convex_get_address(convex, "#9", 9, &address);
    assert(result == CONVEX_OK);
    assert(address == 9);

    result = convex_close(convex);
    assert(result == CONVEX_OK);

    return 0;

}
