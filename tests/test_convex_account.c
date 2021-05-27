
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include <convex_account.h>

int main() {
    convex_account_p account;
    int result;

    // test full init
    account = convex_account_init();
    assert(account);
    result = convex_account_close(account);
    assert(result == CONVEX_OK);

    // export to PEM string
    char buffer[1024];
    long buffer_size = 1024;
    char *password = "secret";

    account = convex_account_init();
    assert(account);

    result = convex_account_export_to_text(account, password, buffer, &buffer_size);
    assert(result == CONVEX_OK);

    //    printf("text: %s\n", buffer);
    assert(strstr(buffer, "PRIVATE KEY---"));
    assert(buffer_size > 200);


    const unsigned char *raw_public_key = convex_account_get_raw_public_key(account);
    char public_key_hex[66];
    char *ptr = public_key_hex;
    for (int index; index < 32; index ++ ) {
        sprintf(ptr, "%02x", raw_public_key[index]);
        ptr += 2;
    }
    *ptr = 0;
    // printf("%s\n", public_key_hex);

    // get the public key
    char public_key[65];
    int public_key_length = 65;

    result = convex_account_public_key(account, public_key, &public_key_length);
    // printf("public key: %s\n", public_key);
    assert(result == CONVEX_OK);

    assert(strcmp(public_key, public_key_hex) == 0);

    // export key data to text and re-import

    char export_data[500];
    long export_data_length = 500;
    result = convex_account_export_to_text(account, password, export_data, &export_data_length);
    assert(result == CONVEX_OK);

    convex_account_p import_account;

    import_account = convex_account_init_from_text(export_data, password);
    assert(import_account);

    // are both public keys equal?
    // printf("account public key: %s\n", convex_account_get_public_key(account));
    // printf("import public key: %s\n", convex_account_get_public_key(import_account));
    assert(strcmp(convex_account_get_public_key(account), convex_account_get_public_key(import_account)) == 0);

    result = convex_account_close(import_account);
    assert(result == CONVEX_OK);

    // export key data to file and re-import

    char *temp_key_filename = "/tmp/convex_account_test.pem";
    result = convex_account_export_to_file(account, temp_key_filename, password);
    assert(result == CONVEX_OK);

    import_account = convex_account_init_from_file(temp_key_filename, password);
    assert(import_account);

    assert(strcmp(convex_account_get_public_key(account), convex_account_get_public_key(import_account)) == 0);

    result = convex_account_close(import_account);
    assert(result == CONVEX_OK);

    result = convex_account_close(account);
    assert(result == CONVEX_OK);

    remove(temp_key_filename);

    return 0;
}

