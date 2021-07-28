
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include <convex_key_pair.h>

int main() {
    convex_key_pair_p key_pair;
    int result;

    // test full init
    key_pair = convex_key_pair_init();
    assert(key_pair);
    result = convex_key_pair_close(key_pair);
    assert(result == CONVEX_OK);


    return 0;
}

