
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include <convex_account.h>

int main() {
    convex_account_p account;
    int result;

    // export to PEM string
    char buffer[1024];
    long buffer_size = 1024;
    const char *password = "secret";

    account = convex_account_init();
    assert(account);

    result = convex_account_export_to_text(account, password, buffer, &buffer_size);
    assert(result == CONVEX_OK);

    //    printf("text: %s\n", buffer);
    assert(strstr(buffer, "PRIVATE KEY---"));
    assert(buffer_size > 200);

    convex_account_p import_account;

    import_account = convex_account_init_from_text(buffer, password);
    assert(import_account);

    // are both public keys equal?
    // printf("account public key: %s\n", convex_account_get_public_key(account));
    // printf("import public key: %s\n", convex_account_get_public_key(import_account));
    assert(strcmp(convex_account_get_public_key(account), convex_account_get_public_key(import_account)) == 0);

    result = convex_account_close(import_account);
    assert(result == CONVEX_OK);

    result = convex_account_close(account);
    assert(result == CONVEX_OK);

    return 0;
}

