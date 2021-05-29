/**
 *
 * Convex Utils
 *
 *
 */

#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/bio.h>

#include "convex_error.h"


void caclulate_hash_sha3_256(const unsigned char *data, const int data_length, unsigned char *hash_data, unsigned int *hash_length) {

    const EVP_MD *md = EVP_sha3_256();
    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(mdctx, md, NULL);
    EVP_DigestUpdate(mdctx, data, data_length);
    EVP_DigestFinal_ex(mdctx, hash_data, hash_length);
    EVP_MD_CTX_free(mdctx);
}


/**
 * Get the public key as a hex string.
 *
 * @param[in] account The account to get the public hex.
 *
 * @param[out] buffer Buffer data to write the public hex string too.
 *
 * @param[out] buffer_length The length of the buffer before setting the string. After calling
 * this function buffer_length is set to the length of the hex string writtern. So this will be
 * set too (CONVEX_ACCOUNT_PUBLIC_KEY_LENGTH *2) + 1
 *
 */
int convex_utils_public_key_to_hex(const char *key_bytes, const int key_length, char *buffer, int *buffer_length) {
    int index = 0;
    char *ptr = buffer;

    unsigned char hash_buffer[key_length * 2];
    int hash_buffer_length = key_length * 2;
    caclulate_hash_sha3_256(key_bytes, key_length, hash_buffer, &hash_buffer_length);

    // check to see if we have enought memory to write too.
    if (*buffer_length < ((key_length * 2) + 1)) {
        *buffer_length = ((key_length * 2) + 1);
        return CONVEX_ERROR_INVALID_PARAMETER;
    }

    int hash_index = 0;
    int value_index = 0;
    for ( index = 0; index < key_length * 2; index ++) {
        unsigned int hash_value = hash_buffer[hash_index];
        unsigned int value = key_bytes[value_index];
        if ((index % 2) == 0) {
            hash_value = hash_value >> 4;
            value = value >> 4;
        }
        hash_value = hash_value & 0x0F;
        value = value & 0x0F;
        if ((hash_value & 0x0F) > 7) {
            sprintf(ptr, "%01X", value);
        }
        else {
            sprintf(ptr, "%01x", value);
        }
        ptr ++;
        if (index % 2) {
            value_index ++;
            hash_index ++;
        }
    }
    *ptr = 0;
    *buffer_length = (key_length * 2) + 1;
    return CONVEX_OK;
}
