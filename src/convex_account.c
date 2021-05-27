/*
 *
 *
 *     convex_account.c
 *
 */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "convex_account.h"

convex_account_p init_empty_account() {
    convex_account_p account = (convex_account_p) malloc(sizeof(convex_account_t));
    if (!account) {
        return NULL;
    }
    memset(account, 0, sizeof(convex_account_t));
    return account;
}


EVP_PKEY_CTX *pkey_ctx_init() {
    EVP_PKEY_CTX *ctx;
    ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_ED25519, NULL);
    if (!ctx) {
        return NULL;
    }
    EVP_PKEY_keygen_init(ctx);

    return ctx;
}

void set_raw_public_key(convex_account_p account) {
    long key_length = sizeof(account->public_key);
    EVP_PKEY_get_raw_public_key(account->key, account->public_key, &key_length);

}


// public functions



/**
 * Init a convex account. This will generate a new public/private ED25991 keys.
 *
 */
convex_account_p convex_account_init() {
    convex_account_p account = init_empty_account();
    if (!account) {
        return NULL;
    }

    EVP_PKEY_CTX *ctx = pkey_ctx_init();
    if (!ctx) {
        free(account);
        return NULL;
    }

    EVP_PKEY_keygen_init(ctx);
    EVP_PKEY_keygen(ctx, &account->key);
    set_raw_public_key(account);
    EVP_PKEY_CTX_free(ctx);
    return account;
}

convex_account_p convex_account_init_from_text(const char *key_text, const char *password) {
    convex_account_p account = init_empty_account();
    if (!account) {
        return NULL;
    }

    const EVP_CIPHER *cipher = EVP_aes_256_cfb();
    if (!cipher) {
        return CONVEX_ACCOUNT_SSL_ERROR;
    }

    BIO *memory = BIO_new(BIO_s_mem());
    BIO_puts(memory, key_text);
    account->key = PEM_read_bio_PrivateKey(memory, NULL, NULL, (const char *)password);
    BIO_free_all(memory);
    if (!account->key) {
        return NULL;
    }
    set_raw_public_key(account);
    return account;
}

convex_account_p convex_account_init_from_file(const char *filename, const char *password) {
    convex_account_p account = init_empty_account();
    if (!account) {
        return NULL;
    }

    FILE *fp = fopen(filename, "r");
    account->key = PEM_read_PrivateKey(fp, NULL, NULL, (const char *)password);
    fclose(fp);
    if (!account->key) {
        return NULL;
    }
    set_raw_public_key(account);
    return account;
}


int convex_account_close(convex_account_p account) {
    if (account) {
        if (account->key) {
            EVP_PKEY_free(account->key);
        }
        free(account);
    }
    return CONVEX_OK;
}

int convex_account_export_to_file(convex_account_p account, const char *filename, const char *password) {

    if (!(password && strlen(password) > 0)) {
        return CONVEX_ERROR_INVALID_PARAMETER;
    }

    const EVP_CIPHER *cipher = EVP_aes_256_cfb();
    if (!cipher) {
        return CONVEX_ACCOUNT_SSL_ERROR;
    }

    FILE *fp = fopen(filename, "w+");
    PEM_write_PKCS8PrivateKey(fp, account->key, cipher, NULL, 0, 0, password);
    fclose(fp);

    return CONVEX_OK;
}

int convex_account_export_to_text(convex_account_p account, const char *password, char *buffer, long *buffer_size) {

    if (!(password && strlen(password) > 0)) {
        return CONVEX_ERROR_INVALID_PARAMETER;
    }

    if (buffer == NULL) {
        return CONVEX_ERROR_INVALID_PARAMETER;
    }

    if (*buffer_size == 0) {
        return CONVEX_ERROR_INVALID_PARAMETER;
    }

    const EVP_CIPHER *cipher = EVP_aes_256_cfb();
    if (!cipher) {
        return CONVEX_ACCOUNT_SSL_ERROR;
    }

    BIO *memory = BIO_new(BIO_s_mem());

    // char *kstr = strdup(password);
    PEM_write_bio_PKCS8PrivateKey(memory, account->key, cipher, NULL, 0, 0, password);
    // free(kstr);

    BUF_MEM *bptr;
    long data_length = BIO_get_mem_data(memory, &bptr);
    *buffer_size = data_length;
    if (data_length > *buffer_size) {
        BIO_free_all(memory);
        return CONVEX_ERROR_INVALID_PARAMETER;
    }
    memcpy(buffer, bptr, data_length);

    BIO_free_all(memory);
    return CONVEX_OK;
}

const unsigned char *convex_account_get_raw_public_key(convex_account_p account) {
    return account->public_key;
}

int convex_account_public_key(convex_account_p account, char *buffer, int *buffer_length) {
    int index = 0;
    char *ptr = buffer;

    // check to see if we have enought memory to write too.
    if (*buffer_length < ((CONVEX_ACCOUNT_PUBLIC_KEY_LENGTH * 2) + 1)) {
        return CONVEX_ERROR_INVALID_PARAMETER;
    }
    for ( index = 0; index < CONVEX_ACCOUNT_PUBLIC_KEY_LENGTH; index ++ ) {
        sprintf(ptr, "%02x", account->public_key[index]);
        ptr += 2;
    }
    *ptr = 0;
    *buffer_length = (CONVEX_ACCOUNT_PUBLIC_KEY_LENGTH * 2) + 1;
    return CONVEX_OK;
}

const char *convex_account_get_public_key(convex_account_p account) {
    int key_length = sizeof(account->public_key_hex);
    int result = convex_account_public_key(account, account->public_key_hex, &key_length);
    if (result != CONVEX_OK) {
        return NULL;
    }
    return account->public_key_hex;
}


