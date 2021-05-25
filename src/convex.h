
/*
 *
 *      convex.h
 *
*/


#ifndef CONVEX_H
#define CONVEX_H

#include "convex_error.h"
#include "convex_default.h"
#include <curl/curl.h>
#include <stdbool.h>

#define MAX_SOURCE_SIZE     2048

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
extern int convex_close(convex_p);
extern const char * convex_get_url(convex_p);
extern int convex_query(convex_p, const char *, long);
extern char *convex_response_get_data(convex_p);
extern bool convex_is_response(convex_p);
extern int convex_response_clear(convex_p);
extern long convex_response_get_code(convex_p);

#endif              // CONVEX_H
