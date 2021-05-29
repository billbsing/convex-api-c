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

/**
 * @mainpage convex-api-c library
 *
 * This libary provides access to the convex network.
 *
 * This library uses the following dependent libararies:
 *
 * + openssl
 * + libcurl
 *
 * See `convex.c` for the API documentation.
 *
 * See an example of using the convex-api-c library:
 *
 * \include example_convex_query.c
 *
 *
 *
 */

/**
 * @private
 *
 * Helper function to create a valid request post data string to send to convex API.
 *
 */
char *create_data_string(const char *source, long address, const char *language) {

    if (strlen(source) > MAX_SOURCE_SIZE) {
        return NULL;
    }
    char *data = malloc(MAX_SOURCE_SIZE + 100);
    sprintf(data, "{\"source\": \"%s\", \"address\": \"#%ld\", \"lang\": \"%s\"}", source, address, language);
    return data;
}

/**
 * @private
 *
 * Clears a response data after calling a `convex_query` or `convex_submit`.
 *
 * @param[in] convex Convex data created by the `convex_init` function.
 *
 * @return CONVEX_OK or CONVEX_ERROR.. if this function was succesfull
 */
const int response_clear(response_p response) {
    if (response->data) {
        free(response->data);
    }
    response->data = NULL;
    response->code = 0;
    return CONVEX_OK;
}

/**
 * @private
 *
 * Call back function, called by curl when the response data is being received.
 *
 */
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

CURLU *convex_create_curl_url(const convex_p convex, const char *uri) {

    CURLUcode result = 0;

    CURLU *url = curl_url();
    if (!url) {
        return NULL;
    }

    result = curl_url_set(url, CURLUPART_URL, convex->url, 0);
    if (result) {
        curl_url_cleanup(url);
        return NULL;
    }
    result = curl_url_set(url, CURLUPART_URL, uri, CURLU_APPENDQUERY);
    if (result) {
        curl_url_cleanup(url);
        return NULL;
    }
    return url;
}

int curl_request_post(CURLU *url, const char *data, response_p response) {

    CURLUcode result = 0;

    struct curl_slist *header_list = NULL;
    header_list = curl_slist_append(header_list, "Content-Type: application/json");

    CURL *curl = curl_easy_init();
    result = curl_easy_setopt(curl, CURLOPT_CURLU, url);
    result = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);
    result = curl_easy_setopt(curl, CURLOPT_POST, 1L);
    result = curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(data));
    result = curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
    result = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    result = curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)response);
    result = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    result = curl_easy_perform(curl);
    if (result == CURLE_OK) {
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response->code);
    }
    curl_slist_free_all(header_list);
    curl_easy_cleanup(curl);
    return CONVEX_OK;

}

// public functions

/**
 * Init a convex connection to a specified url.
 *
 * @param[in] url URL to connect to the convex network. Defaluts: https:\\convex.world
 *
 * @returns convex_p Convex handle that has to be passed to other functions.
 *
 */
convex_p convex_init(const char *url) {
    if (!url) {
        url = CONVEX_DEFAULT_URL;
    }
    convex_p convex = (convex_p) malloc(sizeof(convex_t));
    if (!convex) {
        return NULL;
    }
    memset(convex, 0, sizeof(convex_t));
    convex->url = strdup(url);
    return convex;
}

/**
 * Closes a convex , and frees the memory saved with the convex data.
 *
 * @param[in] convex Convex data created by the `convex_init` function.
 *
 * @returns CONVEX_OK if free memmory was successfull.
 *
 */
int convex_close(convex_p convex) {
    if (convex) {
        response_clear(&convex->response);
        free(convex->url);
        free(convex);
    }
    return CONVEX_OK;
}

/**
 * Return the url set when the convex_init was first called.
 *
 * @param[in] convex Convex data created by the `convex_init` function.
 *
 * @return const char * string of the url
 *
 */
const char * convex_get_url(convex_p convex) {
    if (convex) {
        return convex->url;
    }
    return NULL;
}

const int convex_create_account(const convex_p convex, const convex_account_p account, unsigned long *address) {
    response_t response;
    memset(&response, 0, sizeof(response_t));

    if (!convex) {
        return CONVEX_ERROR_INVALID_PARAMETER;
    }
    if (!account) {
        return CONVEX_ERROR_INVALID_PARAMETER;
    }
    if (!address) {
        return CONVEX_ERROR_INVALID_PARAMETER;
    }
    *address = 0;

    CURLU *url = convex_create_curl_url(convex, "/api/v1/createAccount");
    if (!url) {
        return CONVEX_ERROR_INVALID_URL;
    }

    char data[120];
    sprintf(data, "{\"accountKey\": \"%s\"}", convex_account_get_public_key(account));
    printf("data: %s\n", data);

    int result = curl_request_post(url, data, &response);
    printf("result: %s\n", response.data);
    if (response.code == 200) {
        // {"address":170}
        const char *address_find_text = "{\"address\":";
        char *start_str = strstr(response.data, address_find_text);
        char *end_str = strrchr(response.data, '}');

        if (start_str && end_str) {
	    int start_position = (start_str - response.data) + strlen(address_find_text);
	    int end_position = (end_str - start_str);
	    int length = end_position - start_position;
	    // printf("> %s %s %d\n", start_str, end_str, end_str - start_str); 
	    // printf("pos %d; %d; %d\n", start_position, end_position, length);
            char addressText[length + 10];
            strncpy(addressText, &response.data[start_position], length);
            addressText[length] = 0;
            *address = atoi(addressText);
        }
    }
    response_clear(&response);
    return CONVEX_OK;
}
/**
 * Execute a query on the convex network.
 *
 * @param[in] convex Convex data created by the `convex_init` function.
 *
 * @param[in] query The query string to execute.
 *
 * @param[in] address Optional address of the account to execute the query on. Defaults: 9.
 *
 * @return CONVEX_OK or CONVEX_ERROR.. if this function was succesfull
 *
 */
const int convex_query(const convex_p convex, const char *query, const unsigned long address) {

    if (!convex) {
        return CONVEX_ERROR_INVALID_PARAMETER;
    }
    if (!query) {
        return CONVEX_ERROR_INVALID_PARAMETER;
    }

    unsigned long query_address = CONVEX_DEFAULT_QUERY_ADDRESS;
    if (address) {
        query_address = address;
    }

    CURLU *url = convex_create_curl_url(convex, "/api/v1/query");
    if (!url) {
        return CONVEX_ERROR_INVALID_URL;
    }

    char * data = create_data_string(query, query_address, CONVEX_DEFAULT_LANGUAGE);
    if (!data) {
        curl_url_cleanup(url);
        return CONVEX_ERROR_INVALID_DATA;
    }
    response_clear(&convex->response);

    /*
    char *url_full;
    curl_url_get(url, CURLUPART_URL, &url_full, 0);
    curl_free(url_full);
    */

    int result = curl_request_post(url, data, &convex->response);
    free(data);
    curl_url_cleanup(url);

    return result;
}

/**
 * Get the response data after calling a `convex_query` or `convex_submit`.
 *
 * @param[in] convex Convex data created by the `convex_init` function.
 *
 * @returns const char * of the response text.
 */
const char * convex_response_get_data(const convex_p convex) {
    return convex->response.data;
}

/**
 * Tests to see if a response was returned after calling a `convex_query` or `convex_submit`.
 *
 * @param[in] convex Convex data created by the `convex_init` function.
 *
 * @returns true if a response is returned.
 */
const bool convex_is_response(const convex_p convex) {
    return convex->response.size > 0;
}

/**
 * Get the response code after calling a `convex_query` or `convex_submit`.
 *
 * @param[in] convex Convex data created by the `convex_init` function.
 *
 * @returns long response code. Normally this will be 200 if request was succesfull.
 */
const long convex_response_get_code(const convex_p convex) {
    return convex->response.code;
}
