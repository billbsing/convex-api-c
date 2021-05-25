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
#include "convex.h"

convex_p convex_init() {
	convex_p convex = malloc(sizeof(convex_t));
	return convex_p;
}

convex_close(convex_p convex) {
	if (convex) {
		free(convex);
	}
}
