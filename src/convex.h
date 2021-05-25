
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
 *
 */


#ifndef CONVEX_H
#define CONVEX_H

#include "convex_error.h"

typedef struct {
	long handle;
	
} convex_t;

typedef convex_t * convex_p;


extern int convex_init(convex_p *);
extern int convex_close(convex_p);

#endif			// CONVEX_H
