
/*
 *
 *      convex_account.h
 *
*/


#ifndef CONVEX_KEY_PAIR_H
#define CONVEX_KEY_PAIR_H

#include <stdbool.h>
#include "convex_error.h"
#include "convex_default.h"
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/bio.h>

#define CONVEX_KEY_PAIR_PUBLIC_KEY_LENGTH            32
#define CONVEX_KEY_PAIR_PUBLIC_KEY_HEX_LENGTH        (CONVEX_KEY_PAIR_PUBLIC_KEY_LENGTH * 2)

typedef struct {
    EVP_PKEY *key;
    unsigned char public_key[CONVEX_KEY_PAIR_PUBLIC_KEY_LENGTH];
    char public_key_hex[CONVEX_KEY_PAIR_PUBLIC_KEY_HEX_LENGTH + 1];
} convex_key_pair_t;

typedef convex_key_pair_t* convex_key_pair_p;



extern convex_key_pair_p convex_key_pair_init();
extern convex_key_pair_p convex_key_pair_init_from_text(const char *, const char *);
extern convex_key_pair_p convex_key_pair_init_from_file(FILE *, const char *);
extern int convex_key_pair_close(convex_key_pair_p);
extern int convex_key_pair_export_to_file(convex_key_pair_p, const char *, FILE *);
extern int convex_key_pair_export_to_text(convex_key_pair_p, const char *, char *, long *);
extern const unsigned char *convex_key_pair_get_public_key_bytes(convex_key_pair_p account);
extern int convex_key_pair_public_key(convex_key_pair_p account, char *, int *);
extern const char *convex_key_pair_get_public_key(convex_key_pair_p);
extern const bool convex_key_pair_is_equal(convex_key_pair_p, convex_key_pair_p);
extern int convex_key_pair_sign_data(convex_key_pair_p, const void *, int, unsigned char *, int *);


#endif              // CONVEX_KEY_PAIR_H
