/*
 *
 *
 *     convex_key_pair.c
 *
 */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "convex_key_pair.h"
#include "convex_utils.h"

/**
 * @private
 *
 * init an empty convex key_pair structure and return it's pointer.
 *
 * @return convex_key_pair_p Pointer to a new convex_key_pair_t structure.
 *
 */
convex_key_pair_p init_empty_key_pair() {
    convex_key_pair_p key_pair = (convex_key_pair_p) malloc(sizeof(convex_key_pair_t));
    if (!key_pair) {
        return NULL;
    }
    memset(key_pair, 0, sizeof(convex_key_pair_t));
    return key_pair;
}

/**
 * @private
 *
 * Set's the internal public key bytes to the actual key in the openssl key.
 *
 */
void set_raw_public_key(convex_key_pair_p key_pair) {
    long key_length = sizeof(key_pair->public_key);
    EVP_PKEY_get_raw_public_key(key_pair->key, key_pair->public_key, &key_length);
    memset(key_pair->public_key_hex, 0, sizeof(key_pair->public_key_hex));
}

// public functions



/**
 * Init a convex key_pair. This will generate a new random public/private ED25991 keys.
 *
 */
convex_key_pair_p convex_key_pair_init() {
    EVP_PKEY_CTX *ctx;

    // create an key_pair record
    convex_key_pair_p key_pair = init_empty_key_pair();
    if (!key_pair) {
        return NULL;
    }

    // create the openssl ctx for ED25519 keys
    ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_ED25519, NULL);
    if (!ctx) {
        convex_key_pair_close(key_pair);
        return NULL;
    }

    EVP_PKEY_keygen_init(ctx);

    // generate a new key
    EVP_PKEY_keygen(ctx, &key_pair->key);
    set_raw_public_key(key_pair);

    // now free the openssl ctx
    EVP_PKEY_CTX_free(ctx);
    return key_pair;
}

/**
 * Init a convex key_pair from a PEM text string. This string has been created by the `convex_key_pair_export_to_file`
 * or by the `convex_key_pair_export_text` function.
 *
 * @param[in] key_text PEM string to import.
 *
 * @param[in] password Password string that was used to encrypt the key in PEM format.
 *
 * @return A new convex_key_pair_p value. If the password is incorrect or their is not enought memory,
 * this function will return a NULL.
 *
 */
convex_key_pair_p convex_key_pair_init_from_text(const char *key_text, const char *password) {
    convex_key_pair_p key_pair = init_empty_key_pair();
    if (!key_pair) {
        return NULL;
    }

    BIO *memory = BIO_new(BIO_s_mem());
    BIO_puts(memory, key_text);
    key_pair->key = PEM_read_bio_PrivateKey(memory, NULL, NULL, (void *)password);
    BIO_free_all(memory);
    if (!key_pair->key) {
        convex_key_pair_close(key_pair);
        return NULL;
    }
    set_raw_public_key(key_pair);
    return key_pair;
}

/**
 * Init a convex key_pair data struct using a file handle. The file is opened to a PEM file.
 *
 * @param[in] fp File handle that is open to a PEM file.
 *
 * @param[in] password Password string that was used to encrypt the PEM file.
 *
 * @return The convex_key_pair_p of the key_pair. Return NUL if the file cannot be opened, memory issues
 * or the password is incorrect.
 *
 */
convex_key_pair_p convex_key_pair_init_from_file(FILE *fp, const char *password) {
    convex_key_pair_p key_pair = init_empty_key_pair();
    if (!key_pair) {
        return NULL;
    }

    key_pair->key = PEM_read_PrivateKey(fp, NULL, NULL, (void *)password);
    if (!key_pair->key) {
        convex_key_pair_close(key_pair);
        return NULL;
    }
    set_raw_public_key(key_pair);
    return key_pair;
}

/**
 * Closes and frees the allocated memory for the convex_key_pair data structure.
 *
 * @param[in] key_pair Pointer to the convex_key_pair data.
 *
 * @return CONVEX_OK if ok.
 *
 */
int convex_key_pair_close(convex_key_pair_p key_pair) {
    if (key_pair) {
        if (key_pair->key) {
            EVP_PKEY_free(key_pair->key);
        }
        free(key_pair);
    }
    return CONVEX_OK;
}

/**
 * Exports the public/private key to an encrypted file.
 *
 * @param[in] key_pair The key_pair to export.
 *
 * @param[in] password The string password to encrypt the PEM file.
 *
 * @param[in] fp The file handle to write the encrypted data in PEM format.
 *
 * @return CONVEX_OK if the file was exported.
 *
 */
int convex_key_pair_export_to_file(convex_key_pair_p key_pair, const char *password, FILE *fp) {

    if (!(password && strlen(password) > 0)) {
        return CONVEX_ERROR_INVALID_PARAMETER;
    }

    const EVP_CIPHER *cipher = EVP_aes_256_cfb();
    if (!cipher) {
        return CONVEX_ERROR_SSL_CANNOT_FIND_CIPHER;
    }

    PEM_write_PKCS8PrivateKey(fp, key_pair->key, cipher, NULL, 0, 0, (void *)password);

    return CONVEX_OK;
}

/**
 * Export the public/private key to a PEM encoded text format.
 *
 * @param[in] key_pair key_pair to export.
 *
 * @param[in] password Password to encrypt the PEM format.
 *
 * @param[out] buffer Pointer to a buffer to write too. You must have > 140 bytes created for the PEM to be writtern.
 *
 * @param[out] buffer_size Pointer to a long buffer size. This value must be set to the maximum allowed size the buffer can handle.
 * If less then allowed then this call will return with an error. The function will write the length of data writtern to buffer.
 *
 */
int convex_key_pair_export_to_text(convex_key_pair_p key_pair, const char *password, char *buffer, long *buffer_size) {

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
        return CONVEX_ERROR_SSL_CANNOT_FIND_CIPHER;
    }

    BIO *memory = BIO_new(BIO_s_mem());

    // char *kstr = strdup(password);
    PEM_write_bio_PKCS8PrivateKey(memory, key_pair->key, cipher, NULL, 0, 0, (void *)password);
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
 * @param[in] key_pair key_pair to get the public key in bytes
 *
 * @return The pointer to the public key. The key length is CONVEX_KEY_PAIR_PUBLIC_KEY_LENGTH
 *
 */
const unsigned char *convex_key_pair_get_public_key_bytes(convex_key_pair_p key_pair) {
    return key_pair->public_key;
}

/**
 * Return a hex string of the public key.
 *
 * @param[in] key_pair key_pair to get the public key.
 *
 * @return a hex string of the public key.
 *
 */
const char *convex_key_pair_get_public_key(convex_key_pair_p key_pair) {
    int key_length = sizeof(key_pair->public_key_hex);
    if (strlen(key_pair->public_key_hex) == 0) {
        int result = convex_utils_public_key_to_hex(key_pair->public_key, CONVEX_KEY_PAIR_PUBLIC_KEY_LENGTH, key_pair->public_key_hex, &key_length);
        if (result != CONVEX_OK) {
            return NULL;
        }
    }
    return key_pair->public_key_hex;
}


/**
 * Return true if both key_pairs are equal.
 *
 * @param[in] a key_pair a to compare.
 *
 * @param[in] b key_pair b to compare with key_pair a.
 *
 * @return True if both public keys are equal.
 *
 */
const bool convex_key_pair_is_equal(convex_key_pair_p a, convex_key_pair_p b) {
    int index;
    const unsigned char *ptr_a = convex_key_pair_get_public_key_bytes(a);
    const unsigned char *ptr_b = convex_key_pair_get_public_key_bytes(b);
    return memcmp(ptr_a, ptr_b, CONVEX_KEY_PAIR_PUBLIC_KEY_LENGTH) == 0;
}

/**
 * Sign hash and return the signature.
 *
 * @param[in] key_pair key_pair that can do the signing.
 *
 * @param[in] hash_data Hash data as bytes.
 *
 * @param[in] hash_data_length Length of the hash data to sign.
 *
 * @param[out] signed_data Signed data to write in bytes.
 *
 * @param[out] signed_data_length Length of the sigend data buffer, after this call,
 * the number of bytes writtern by the signing process.
 *
 * @return CONVEX_OK if the signing was successfull.
 *
 */
int convex_key_pair_sign_data(
        convex_key_pair_p key_pair,
        const void *hash_data,
        int hash_data_length,
        unsigned char *signed_data,
        int *signed_data_length
    ) {

    if (!signed_data_length) {
        return CONVEX_ERROR_INVALID_PARAMETER;
    }

    int sign_length = EVP_PKEY_size(key_pair->key);
    if ( sign_length > *signed_data_length ) {
        *signed_data_length = sign_length;
        return CONVEX_ERROR_SIGN_DATA_TOO_SMALL;
    }

    EVP_PKEY_CTX *key_ctx = NULL;
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    if (!ctx) {
        return CONVEX_ERROR_SSL_CANNOT_INIT;
    }

    size_t signed_length = *signed_data_length;
    EVP_DigestSignInit(ctx, &key_ctx, NULL, NULL, key_pair->key);
    EVP_DigestSign(ctx, signed_data, &signed_length, hash_data, (size_t) hash_data_length);
    EVP_MD_CTX_free(ctx);
    *signed_data_length = (int) signed_length;
    return CONVEX_OK;
}
