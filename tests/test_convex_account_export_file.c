
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include <convex_account.h>

int main() {
    convex_account_p account;
    int result;
    const char *password = "secret";

    // export key data to file and re-import
    account = convex_account_init();
    assert(account);

    char *temp_key_filename = "/tmp/convex_account_test.pem";
    FILE *fp;
    fp = fopen(temp_key_filename, "w+");
    result = convex_account_export_to_file(account, password, fp);
    assert(result == CONVEX_OK);
    fclose(fp);


    fp = fopen(temp_key_filename, "r");
    convex_account_p import_account = convex_account_init_from_file(fp, password);
    assert(import_account);
    fclose(fp);

    assert(strcmp(convex_account_get_public_key(account), convex_account_get_public_key(import_account)) == 0);
    assert(convex_account_is_equal(account, import_account));

    result = convex_account_close(import_account);
    assert(result == CONVEX_OK);

    result = convex_account_close(account);
    assert(result == CONVEX_OK);

    remove(temp_key_filename);

    return 0;
}

