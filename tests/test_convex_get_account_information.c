/*
 *
 *
 * test_convex_get_account_information
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

    result = convex_get_account_information(convex, 9);
    assert(result == CONVEX_OK);

    printf("info result: %s\n", convex->response.data);

    result = convex_close(convex);
    assert(result == CONVEX_OK);

}
