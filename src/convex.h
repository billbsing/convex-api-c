
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

typedef struct {
    long handle;
    char *url;
    CURL *curl;
} convex_t;

typedef convex_t * convex_p;


extern int convex_init(convex_p*, const char*);
extern int convex_close(convex_p);
extern int convex_query(convex_p, const char *, int);

#endif              // CONVEX_H
