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
#include <stdlib.h>
#include "convex.h"

int convex_init(convex_p *convex) {
	*convex = (convex_p) malloc(sizeof(convex_t));
	return 0;
}

int convex_close(convex_p convex) {
	if (convex) {
		free(convex);
	}
	return 0;
}
