
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include <convex_key_pair.h>

int main() {
    convex_key_pair_p key_pair;
    int result;

    // export to PEM string
    char buffer[1024];
    long buffer_size = 1024;
    const char *password = "secret";

    key_pair = convex_key_pair_init();
    assert(key_pair);

    result = convex_key_pair_export_to_text(key_pair, password, buffer, &buffer_size);
    assert(result == CONVEX_OK);

    //    printf("text: %s\n", buffer);
    assert(strstr(buffer, "PRIVATE KEY---"));
    assert(buffer_size > 200);

    convex_key_pair_p import_key_pair;

    import_key_pair = convex_key_pair_init_from_text(buffer, password);
    assert(import_key_pair);

    // are both public keys equal?
    // printf("key_pair public key: %s\n", convex_key_pair_get_public_key(key_pair));
    // printf("import public key: %s\n", convex_key_pair_get_public_key(import_key_pair));
    assert(strcmp(convex_key_pair_get_public_key(key_pair), convex_key_pair_get_public_key(import_key_pair)) == 0);

    result = convex_key_pair_close(import_key_pair);
    assert(result == CONVEX_OK);

    result = convex_key_pair_close(key_pair);
    assert(result == CONVEX_OK);

    return 0;
}

