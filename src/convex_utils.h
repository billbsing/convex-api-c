/**
 *
 *
 * Convex Utils
 *
 */


#ifndef CONVEX_UTILS_H
#define CONVEX_UTILS_H

#include "convex_error.h"


extern const int convex_utils_public_key_to_hex(const char *, const int , char *, int *);
extern const int convex_utils_bytes_to_hex(const char *, const int , char *, int *);
extern const int convex_utils_hex_to_bytes(const char *, unsigned char *, int *);


#endif          // CONVEX_UTILS_H
