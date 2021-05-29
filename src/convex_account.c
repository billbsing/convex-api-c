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
#include "convex_utils.h"

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
 * Init a convex account. This will generate a new random public/private ED25991 keys.
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

/**
 * Init a convex account from a PEM text string. This string has been created by the `convex_account_export_to_file`
 * or by the `convex_account_export_text` function.
 *
 * @param[in] key_text PEM string to import.
 *
 * @param[in] password Password string that was used to encrypt the key in PEM format.
 *
 * @return A new convex_account_p value. If the password is incorrect or their is not enought memory,
 * this function will return a NULL.
 *
 */
convex_account_p convex_account_init_from_text(const char *key_text, const char *password) {
    convex_account_p account = init_empty_account();
    if (!account) {
        return NULL;
    }

    BIO *memory = BIO_new(BIO_s_mem());
    BIO_puts(memory, key_text);
    account->key = PEM_read_bio_PrivateKey(memory, NULL, NULL, (const char *)password);
    BIO_free_all(memory);
    if (!account->key) {
        printf("no key\n");
        return NULL;
    }
    set_raw_public_key(account);
    return account;
}

/**
 * Init a convex account data struct using a file handle. The file is opened to a PEM file.
 *
 * @param[in] fp File handle that is open to a PEM file.
 *
 * @param[in] password Password string that was used to encrypt the PEM file.
 *
 * @return The convex_account_p of the account. Return NUL if the file cannot be opened, memory issues
 * or the password is incorrect.
 *
 */
convex_account_p convex_account_init_from_file(FILE *fp, const char *password) {
    convex_account_p account = init_empty_account();
    if (!account) {
        return NULL;
    }

    account->key = PEM_read_PrivateKey(fp, NULL, NULL, (const char *)password);
    if (!account->key) {
        return NULL;
    }
    set_raw_public_key(account);
    return account;
}

/**
 * Closes and frees the allocated memory for the convex_account data structure.
 *
 * @param[in] account Pointer to the convex_account data.
 *
 * @return CONVEX_OK if ok.
 *
 */
int convex_account_close(convex_account_p account) {
    if (account) {
        if (account->key) {
            EVP_PKEY_free(account->key);
        }
        free(account);
    }
    return CONVEX_OK;
}

/**
 * Exports the public/private key to an encrypted file.
 *
 * @param[in] account The account to export.
 *
 * @param[in] password The string password to encrypt the PEM file.
 *
 * @param[in] fp The file handle to write the encrypted data in PEM format.
 *
 * @return CONVEX_OK if the file was exported.
 *
 */
int convex_account_export_to_file(convex_account_p account, const char *password, FILE *fp) {

    if (!(password && strlen(password) > 0)) {
        return CONVEX_ERROR_INVALID_PARAMETER;
    }

    const EVP_CIPHER *cipher = EVP_aes_256_cfb();
    if (!cipher) {
        return CONVEX_ACCOUNT_SSL_ERROR;
    }

    PEM_write_PKCS8PrivateKey(fp, account->key, cipher, NULL, 0, 0, password);

    return CONVEX_OK;
}

/**
 * Export the public/private key to a PEM encoded text format.
 *
 * @param[in] account Account to export.
 *
 * @param[in] password Password to encrypt the PEM format.
 *
 * @param[out] buffer Pointer to a buffer to write too. You must have > 140 bytes created for the PEM to be writtern.
 *
 * @param[out] buffer_size Pointer to a long buffer size. This value must be set to the maximum allowed size the buffer can handle.
 * If less then allowed then this call will return with an error. The function will write the length of data writtern to buffer.
 *
 */
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

/**
 * Pointer to the public key in bytes.
 *
 * @param[in] account Account to get the public key in bytes
 *
 * @return The pointer to the public key. The key length is CONVEX_ACCOUNT_PUBLIC_KEY_LENGTH
 *
 */
const unsigned char *convex_account_get_public_key_bytes(convex_account_p account) {
    return account->public_key;
}

/**
 * Return a hex string of the public key.
 *
 * @param[in] account Account to get the public key.
 *
 * @return a hex string of the public key.
 *
 */
const char *convex_account_get_public_key(convex_account_p account) {
    int key_length = sizeof(account->public_key_hex);
    if (strlen(account->public_key_hex) == 0) {
        int result = convex_utils_public_key_to_hex(account->public_key, CONVEX_ACCOUNT_PUBLIC_KEY_LENGTH, account->public_key_hex, &key_length);
        if (result != CONVEX_OK) {
            return NULL;
        }
    }
    return account->public_key_hex;
}


/**
 * Return true if both accounts are equal.
 *
 * @param[in] a Account a to compare.
 *
 * @param[in] b Account b to compare with account a.
 *
 * @return True if both public keys are equal.
 *
 */
const bool convex_account_is_equal(convex_account_p a, convex_account_p b) {
    int index;
    const unsigned char *ptr_a = convex_account_get_public_key_bytes(a);
    const unsigned char *ptr_b = convex_account_get_public_key_bytes(b);
    return memcmp(ptr_a, ptr_b, CONVEX_ACCOUNT_PUBLIC_KEY_LENGTH) == 0;
}

