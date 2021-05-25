

#include <stdio.h>
#include <convex.h>
#include <assert.h>
#include <string.h>

int main() {
    convex_p convex;
    const char *url_test = "https://convex.test.world";
    int result;

    // test full init
    convex = convex_init(url_test);
    assert(convex);
    assert(strcmp(convex_get_url(convex), url_test) == 0);
    result = convex_close(convex);
    assert(result == CONVEX_OK);

    // test init with default url
    convex = convex_init(NULL);
    assert(convex);
    assert(strcmp(convex_get_url(convex), CONVEX_DEFAULT_URL) == 0);
    result = convex_close(convex);
    assert(result == CONVEX_OK);

    return 0;
}

