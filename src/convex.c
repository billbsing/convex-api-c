/*
 *
 *
 *     Convex.c
 *
 */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "convex.h"

char *create_data_string(const char *source, long address, const char *language) {

    if (strlen(source) > MAX_SOURCE_SIZE) {
        return NULL;
    }
    char *data = malloc(MAX_SOURCE_SIZE + 100);
    sprintf(data, "{\"source\": \"%s\", \"address\": \"#%ld\", \"lang\": \"%s\"}", source, address, language);
    return data;
}

size_t write_callback(char *ptr, size_t size, size_t nmemb, void *user_data) {
    size_t real_size = size * nmemb;
    response_p response = (response_p) user_data;

    char *new_data = (char *) realloc(response->data, response->size + real_size + 1);
    if (!new_data) {
        return 0;
    }
    response->data = new_data;
    memcpy(&response->data[response->size], ptr, real_size);
    response->size += real_size;
    response->data[response->size] = 0;
    return real_size;
}

// public functions

int convex_init(convex_p *convex, const char *url) {
    if (!url) {
        url = CONVEX_DEFAULT_URL;
    }
    if (!convex) {
        return CONVEX_ERROR_INVALID_PARAMETER;
    }
    convex_p data = (convex_p) malloc(sizeof(convex_t));
    memset(data, 0, sizeof(convex_t));
    data->url = strdup(url);
    *convex = data;
    return CONVEX_OK;
}

int convex_close(convex_p convex) {
    if (convex) {
        convex_response_clear(convex);
        free(convex->url);
        free(convex);
    }
    return CONVEX_OK;
}

int convex_query(convex_p convex, const char *query, long address) {
    CURLUcode result = 0;

    if (!convex) {
        return CONVEX_ERROR_INVALID_PARAMETER;
    }
    if (!query) {
        return CONVEX_ERROR_INVALID_PARAMETER;
    }

    if (address <= 0) {
        address = CONVEX_DEFAULT_QUERY_ADDRESS;
    }

    CURLU *url = curl_url();
    if (!url) {
        return CONVEX_ERROR_INVALID_URL;
    }

    result = curl_url_set(url, CURLUPART_URL, convex->url, 0);
    if (result) {
        curl_url_cleanup(url);
        return CONVEX_ERROR_INVALID_URL;
    }
    result = curl_url_set(url, CURLUPART_URL, "/api/v1/query", CURLU_APPENDQUERY);
    if (result) {
        curl_url_cleanup(url);
        return CONVEX_ERROR_INVALID_URL;
    }

    char * data = create_data_string(query, address, CONVEX_DEFAULT_LANGUAGE);
    if (!data) {
        curl_url_cleanup(url);
        return CONVEX_ERROR_INVALID_DATA;
    }
    convex_response_clear(convex);

    char *url_full;
    curl_url_get(url, CURLUPART_URL, &url_full, 0);
    printf("url: %s\n", url_full);
    curl_free(url_full);
    printf("address: %ld\n", address);

    printf("data: %s\n", data);

    struct curl_slist *header_list = NULL;
    header_list = curl_slist_append(header_list, "Content-Type: application/json");

    CURL *curl = curl_easy_init();
    result = curl_easy_setopt(curl, CURLOPT_CURLU, url);
    result = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);
    result = curl_easy_setopt(curl, CURLOPT_POST, 1L);
    result = curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(data));
    result = curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
    result = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    result = curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&convex->response);
    result = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    result = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &convex->response.code);
    result = curl_easy_perform(curl);

    curl_slist_free_all(header_list);
    free(data);
    curl_easy_cleanup(curl);
    curl_url_cleanup(url);

    return CONVEX_OK;
}

char * convex_response_get_data(convex_p convex) {
    return convex->response.data;
}

bool convex_is_response(convex_p convex) {
    return convex->response.size > 0;
}

int convex_response_clear(convex_p convex) {
    if (convex->response.data) {
        free(convex->response.data);
    }
    convex->response.data = NULL;
    convex->response.code = 0;
    return CONVEX_OK;
}

long convex_response_get_code(convex_p convex) {
    return convex->response.code;
}
