/*
 *
 *
 *
 *
 */

#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/bio.h>
#include <assert.h>


int main() {
    // test ssl key gen

    /*
    ENGINE *engine = ENGINE_get_first();
    while (engine) {
        printf("engine: %s", ENGINE_get_name(engine));
        engine = ENGINE_get_next(engine);
    }
    return 0;
    */

    EVP_PKEY *key = NULL;
    EVP_PKEY_CTX *ctx;

    ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_ED25519, NULL);
    assert(ctx);

    EVP_PKEY_keygen_init(ctx);

    const EVP_CIPHER *cipher = EVP_aes_256_cfb();

    assert(cipher);
    // generate key
    EVP_PKEY_keygen(ctx, &key);
    printf("generate key \n");

    FILE *fp = fopen("test_key_private.pem", "wb+");
    char *password = "secret";

    PEM_write_PKCS8PrivateKey(fp, key, cipher, password, strlen(password), NULL, NULL);

    fclose(fp);

    size_t public_key_length = 32;
    unsigned char public_key[public_key_length];

    EVP_PKEY_get_raw_public_key(key, public_key, &public_key_length);

    for (uint8_t index; index < public_key_length; index ++ ) {
        printf("%02x:", public_key[index]);
    }
    printf("\n");

    BIO *memory = BIO_new(BIO_s_mem());
    EVP_PKEY_print_public(memory, key, 0, NULL);
    // PEM_write_bio_PUBKEY(memory, key);

    uint8_t *public_data = NULL;
    long data_length = BIO_get_mem_data(memory, &public_data);
    for (uint8_t index; index < data_length; index ++ ) {
        printf("%c", public_data[index]);
    }
    BIO_free_all(memory);
    /*


    fp = fopen("test_key_public.pem", "wb+");
    PEM_write_PUBKEY(fp, key);
    fclose(fp);

    printf("\n");

    */

    EVP_PKEY_free(key);
    EVP_PKEY_CTX_free(ctx);

    // EVP_PKEY_ED25519
    // EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_from_name(NULL, "ED25519", NULL);
    // EVP_PKEY *key = EVP_PKEY_Q_keygen(NULL, NULL, "ED25519");
    /*
    EVP_PKEY_keygen_init(ctx);

    EVP_PKEY_keygen(ctx, &key);

    assert(key);

    */
}
