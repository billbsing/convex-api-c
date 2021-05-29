
/*
 *
 *      convex.h
 *
*/


#ifndef CONVEX_H
#define CONVEX_H

#include "convex_error.h"
#include "convex_default.h"
#include "convex_account.h"
#include <curl/curl.h>
#include <stdbool.h>

#define MAX_SOURCE_SIZE     (1024 * 1024)

typedef struct {
    char *data;
    size_t size;
    long code;
} response_t;

typedef response_t* response_p;


typedef struct {
    long handle;
    char *url;
    response_t response;
} convex_t;

typedef convex_t* convex_p;



extern convex_p convex_init(const char*);
extern const int convex_close(convex_p);
extern const char * convex_get_url(const convex_p);
extern const int convex_create_account(const convex_p convex, const convex_account_p account, unsigned long *address);
extern const int convex_query(const convex_p, const char *, const unsigned long);
extern const char *convex_response_get_data(const convex_p);
extern const bool convex_is_response(const convex_p);
extern const long convex_response_get_code(const convex_p);

#endif              // CONVEX_H
