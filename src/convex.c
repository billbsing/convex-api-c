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
#include <string.h>
#include "convex.h"

int convex_init(convex_p *convex, const char *url) {
	if (!url) {
		url = CONVEX_DEFAULT_URL;
	}
	if (!convex) {
		return CONVEX_ERROR_INVALID_PARAMETER;
	}
	convex_p data = (convex_p) malloc(sizeof(convex_t));
	data->url = strdup(url);
	data->curl = curl_easy_init();
	*convex = data;
	return CONVEX_OK;
}

int convex_close(convex_p convex) {
	if (convex) {
		curl_easy_cleanup(convex->curl);
		free(convex->url);
		free(convex);
	}
	return CONVEX_OK;
}

int convex_query(convex_p convex, const char *query, int address) {
	if (!convex) {
		return CONVEX_ERROR_INVALID_PARAMETER;
	}
	if (!query) {
		return CONVEX_ERROR_INVALID_PARAMETER;
	}
	return CONVEX_OK;
}
