#include <convex.h>

/*
 * Example to do a basic convex_query
 *
 */
int main() {
    // create a new convex data handle.
    convex_p convex = convex_init("https://convex.world");
    if (!convex) {
        printf("cannot start convex client\n");
        return 1;
    }

    // call a query , if no account number is given (<=0) it defaults to 9.
    int result = convex_query(convex, "*balance*", 0L);
    if (result != CONVEX_OK) {
        printf("convex_query returned an error %d\n", result);
        convex_close(convex);
        return result;
    }

    // check to see if a valid response has returned
    if (convex_is_response(convex) && convex_response_get_code(convex) == 200) {
        // print out the query response
        printf("convex_query result: %s\n", convex_response_get_data(convex));
    }

    // clear and close the convex data.
    convex_close(convex);
    return 0;
}
