
/*
 *
 *      convex_account.h
 *
*/


#ifndef CONVEX_ACCOUNT_H
#define CONVEX_ACCOUNT_H

#include <stdbool.h>
#include "convex_error.h"
#include "convex_default.h"
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/bio.h>

#define CONVEX_ACCOUNT_PUBLIC_KEY_LENGTH            32
#define CONVEX_ACCOUNT_PUBLIC_KEY_HEX_LENGTH        (CONVEX_ACCOUNT_PUBLIC_KEY_LENGTH * 2)

typedef struct {
    EVP_PKEY *key;
    unsigned char public_key[CONVEX_ACCOUNT_PUBLIC_KEY_LENGTH];
    char public_key_hex[(CONVEX_ACCOUNT_PUBLIC_KEY_LENGTH * 2) + 1];
} convex_account_t;

typedef convex_account_t* convex_account_p;



extern convex_account_p convex_account_init();
extern convex_account_p convex_account_init_from_text(const char *, const char *);
extern convex_account_p convex_account_init_from_file(FILE *, const char *);
extern int convex_account_close(convex_account_p);
extern int convex_account_export_to_file(convex_account_p, const char *, FILE *);
extern int convex_account_export_to_text(convex_account_p, const char *, char *, long *);
extern const unsigned char *convex_account_get_public_key_bytes(convex_account_p account);
extern int convex_account_public_key(convex_account_p account, char *, int *);
extern const char *convex_account_get_public_key(convex_account_p);
extern const bool convex_account_is_equal(convex_account_p, convex_account_p);


#endif              // CONVEX_ACCOUNT_H
