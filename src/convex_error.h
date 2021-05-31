/*
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */

#ifndef CONVEX_ERROR_H
#define CONVEX_ERROR_H

typedef struct {
    int code;
    const char *message;
} error_message_t;


#define CONVEX_OK                               0x00
#define CONVEX_ERROR_FAIL                       0x01
#define CONVEX_ERROR_INVALID_PARAMETER          0x02
#define CONVEX_ERROR_INVALID_URL                0x03
#define CONVEX_ERROR_INVALID_DATA               0x04
#define CONVEX_ERROR_CURL_INIT                  0x05
#define CONVEX_ERROR_BAD_HASH                   0x06
#define CONVEX_ERROR_PREPARE_FAILED             0x07
#define CONVEX_ERROR_NO_ACCOUNT_ADDRESS         0x08
#define CONVEX_ERROR_CREATE_ACCOUNT_FAILED      0x09
#define CONVEX_ERROR_REQUEST_FUNDS_FAILED       0x0A
#define CONVEX_ERROR_SIGN_DATA_TOO_SMALL        0x0B
#define CONVEX_ERROR_SUBMIT_FAILED              0x0C
#define CONVEX_ERROR_SSL                        0x15
#define CONVEX_ERROR_SSL_CANNOT_INIT            0x11
#define CONVEX_ERROR_SSL_CANNOT_FIND_CIPHER     0x12
#define CONVEX_ERROR_SSL_CANNOT_CREATE_CTX      0x13


#endif            // CONVEX_ERROR_H
