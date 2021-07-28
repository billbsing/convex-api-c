
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include <convex_key_pair.h>

int main() {
    convex_key_pair_p key_pair;
    int result;
    const char *password = "secret";

    // export key data to file and re-import
    key_pair = convex_key_pair_init();
    assert(key_pair);

    char *temp_key_filename = "/tmp/convex_key_pair_test.pem";
    FILE *fp;
    fp = fopen(temp_key_filename, "w+");
    result = convex_key_pair_export_to_file(key_pair, password, fp);
    assert(result == CONVEX_OK);
    fclose(fp);


    fp = fopen(temp_key_filename, "r");
    convex_key_pair_p import_key_pair = convex_key_pair_init_from_file(fp, password);
    assert(import_key_pair);
    fclose(fp);

    assert(strcmp(convex_key_pair_get_public_key(key_pair), convex_key_pair_get_public_key(import_key_pair)) == 0);
    assert(convex_key_pair_is_equal(key_pair, import_key_pair));

    result = convex_key_pair_close(import_key_pair);
    assert(result == CONVEX_OK);

    result = convex_key_pair_close(key_pair);
    assert(result == CONVEX_OK);

    remove(temp_key_filename);

    return 0;
}

