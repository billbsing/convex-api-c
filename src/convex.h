
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

typedef unsigned long address_t;
typedef unsigned long amount_t;


extern convex_p convex_init(const char*);
extern int convex_close(convex_p);
extern const char * convex_get_url(const convex_p);
extern int convex_create_account(const convex_p, const convex_account_p, address_t *);
extern int convex_request_funds(convex_p, const amount_t, const address_t, amount_t *);
extern int convex_get_address(const convex_p, const char *, const address_t, address_t *);
extern int convex_query(convex_p, const char *, const address_t);
extern int convex_send(convex_p, const char *,const convex_account_p, address_t address);
extern int convex_transfer(convex_p, address_t, amount_t, convex_account_p, address_t);
extern int convex_get_account_information(convex_p, const address_t);
extern int convex_get_account_balance(convex_p, const address_t, amount_t *);
extern const char *convex_response_get_data(const convex_p);
extern bool convex_is_response(const convex_p);
extern long convex_response_get_code(const convex_p);

#endif              // CONVEX_H
