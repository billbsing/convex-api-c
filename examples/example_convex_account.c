#include <convex.h>

/*
 * Example to create an account and export the key
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

    // create a new account
    convex_account_p account = convex_accoount_init();

    address_t address;
    result = convex_create_account(convex, account, &address);
    if (result != CONVEX_OK) {
        printf("cannot create a new account address\n");
    }

    printf("create a new account address %d\n", address);

    // save the account keys as a encrypted pem file
    const char *password = "secret";
    FILE *fp = fopen("my_account_key.pem", "w+");
    result = convex_account_export_to_file(account, password, fp);
    fclose(fp);

    // clear and close the convex data.
    convex_close(convex);
    return 0;
}
