/**
 *
 * Convex Utils
 *
 *
 */

#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/bio.h>
#include <string.h>

#include "convex_error.h"

/**
 * Calculate a sha3-256 hash on some data.
 *
 */
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
 * @param[in] key_bytes The public key in bytes.
 *
 * @param[in] key_length The length of the public key.
 *
 * @param[out] buffer Buffer data to write the public hex string too.
 *
 * @param[out] buffer_length The length of the buffer before setting the string. After calling
 * this function buffer_length is set to the length of the hex string writtern.
 * So this will be set too (key_length * 2) + 1.
 *
 * @return CONVEX_OK if the function was successfull.
 *
 */
int convex_utils_public_key_to_hex(const unsigned char *key_bytes, const int key_length, char *buffer, int *buffer_length) {
    int index = 0;
    char *ptr = buffer;

    if (buffer_length == NULL) {
        return CONVEX_ERROR_INVALID_PARAMETER;
    }

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

/**
 * Convert byte data to hex string.
 *
 * @param[in] data Data bytes to read.
 *
 * @param[in] data_length Number of data bytes to convert.
 *
 * @param[out] buffer String buffer to write the hex text.
 *
 * @param[out] buffer_length Total length of the buffer before calling this function, after this function is called
 *  the string length of the buffer.
 *
 * @returns CONVEX_OK if the conversion was successfull.
 *
 */
int convex_utils_bytes_to_hex(const unsigned char *data, const int data_length, char *buffer, int *buffer_length) {
    if (buffer_length == NULL) {
        return CONVEX_ERROR_INVALID_PARAMETER;
    }

    if ( (data_length * 2) + 1 > *buffer_length) {
        *buffer_length = (data_length * 2) + 1;
        return CONVEX_ERROR_INVALID_PARAMETER;
    }
    char *ptr = buffer;
    for (int index = 0; index < data_length; index ++) {
        sprintf(ptr, "%02x", data[index]);
        ptr += 2;
    }
    *buffer_length = (data_length * 2) + 1;
    *ptr = 0;
    return CONVEX_OK;
}

/**
 * Convert hex string to bytes.
 *
 * @param[in] data Hex string to read.
 *
 *
 * @param[out] buffer Data buffer to write the bytes.
 *
 * @param[out] buffer_length Total length of the data buffer before calling this function, after this function is called
 *  the number of bytes writtern.
 *
 * @returns CONVEX_OK if the conversion was successfull.
 *
 */
int convex_utils_hex_to_bytes(const char *data, unsigned char *buffer, int *buffer_length) {
    if (buffer_length == NULL) {
        return CONVEX_ERROR_INVALID_PARAMETER;
    }
    int data_length = strlen(data);
    if ( (data_length / 2) > *buffer_length) {
        *buffer_length = (data_length / 2);
        return CONVEX_ERROR_INVALID_PARAMETER;
    }

    char *ptr = buffer;
    int length = 0;
    for (int index = 0; index < data_length; index += 2) {
        sscanf(&data[index], "%02x", ptr);
        ptr ++;
        length ++;
    }
    *buffer_length = length;
    return CONVEX_OK;
}
