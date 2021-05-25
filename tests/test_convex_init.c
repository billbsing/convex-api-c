

#include <stdio.h>
#include <convex.h>
#include <assert.h>

int main() {
    convex_p convex;
    const char *url = "http://convex.world";
    int result;

    // test full init
    result = convex_init(&convex, url);
    assert(result == CONVEX_OK);
    result = convex_close(convex);
    assert(result == CONVEX_OK);

    // test init with default url
    result = convex_init(&convex, NULL);
    assert(result == CONVEX_OK);
    result = convex_close(convex);
    assert(result == CONVEX_OK);

    // test bad parameters
    result = convex_init(NULL, NULL);
    assert(result != CONVEX_OK);
    return 0;
}

