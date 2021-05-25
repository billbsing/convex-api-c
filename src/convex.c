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
	if (!convex) {
		return CONVEX_INVALID_PARAMETER;
	}
	*convex = (convex_p) malloc(sizeof(convex_t));
	return CONVEX_OK;
}

int convex_close(convex_p convex) {
	if (convex) {
		free(convex);
	}
	return CONVEX_OK;
}
