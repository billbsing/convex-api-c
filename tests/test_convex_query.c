

#include <stdio.h>
#include <convex.h>
#include <assert.h>
#include <string.h>

int main() {
    convex_p convex;
    const char *url = "https://convex.world";
    int result;

    // test full init
    result = convex_init(&convex, url);
    assert(result == CONVEX_OK);

    result = convex_query(convex, "*balance*", -1);

    assert(convex_is_response(convex));
    char *data = convex_response_get_data(convex);
    assert(data);
    if (data) {
        printf("response data: %s\n", data);
    }
    assert(strstr(data, "\"value\":"));
    result = convex_close(convex);
    assert(result == CONVEX_OK);

    return 0;
}

