#include <convex.h>

/*
 * Example to send a transaction
 *
 */
int main() {
    int result;
    // create a new convex data handle.
    convex_p convex = convex_init("https://convex.world");
    if (!convex) {
        printf("cannot start convex client\n");
        return 1;
    }

    // see example_convex_account.c on how to create a new account key and address
    // load the account key from an encrypted pem file
    const char *password = "secret";
    FILE *fp = fopen("my_account_key.pem", "w+");

    // create a new account
    convex_account_p account = convex_account_init_from_file(fp, password);
    fclose(fp);
    if ( !account) {
        printf("cannot open account key\n");
        return 1;
    }

    // see example_convex_account.c on how to create a new account address
    address_t address = 42;
    const char *transaction = "(map inc [1 2 3 4 5])";
    result = convex_send(convex, transaction, account, address);
    if (result != CONVEX_OK) {
        printf("convex_send returned an error %d\n", result);
        convex_close(convex);
        return result;
    }

    // check to see if a valid response has returned
    if (convex_is_response(convex) && convex_response_get_code(convex) == 200) {
        // print out the query response
        printf("convex_send result: %s\n", convex_response_get_data(convex));
    }

    // clear and close the convex data.
    convex_close(convex);
    return 0;
}
