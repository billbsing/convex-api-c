

#include <stdio.h>
#include <convex.h>
#include <assert.h>
#include <string.h>

int main() {
    convex_p convex;
    const char *url = "https://convex.world";
    int result;

    // test query
    convex = convex_init(url);
    assert(convex);

    result = convex_query(convex, "*balance*", 0L);

    assert(convex_is_response(convex));
    assert(convex_response_get_code(convex) == 200L);
    const char *data = convex_response_get_data(convex);
    assert(data);
    if (data) {
        printf("response data: %s\n", data);
    }
    assert(strstr(data, "\"value\":"));
    result = convex_close(convex);
    assert(result == CONVEX_OK);

    return 0;
}

