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

/**
 * @private
 *
 * Create a curl url with the convex network api url and the uri appended to the end.
 *
 * @param[in] convex Convex data
 *
 * @param[in] uri URI to append at the end of the convex api URL.
 *
 * @return CURLU * pointer, you need to call curl_url_cleanup(url), after using the URL.
 *      Can return NULL if the URL cannot be created.
 *
 */
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

/**
 * @private
 *
 * Do a curl POST request with the convex world api.
 *
 * @param[in] url Curl URL with the full convex world api.
 *
 * @param[in] data Post data as a string to send to the api.
 *
 * @param[out] repsonse Response_t record that will be filled in by the request. You need to
 *  call `response_close()` after this to clear out the allocated result string.
 *
 */
int curl_request_post(CURLU *url, const char *data, response_p response) {

    CURLUcode result = 0;

    struct curl_slist *header_list = NULL;
    header_list = curl_slist_append(header_list, "Content-Type: application/json");

    CURL *curl = curl_easy_init();
    if (!curl) {
        return CONVEX_ERROR_CURL_INIT;
    }

    // clear out the response before the request
    response_clear(response);

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

char *get_value_from_json(const char *data, const char *field_name){
    char *value_text = NULL;
    char *field_find_text = malloc(strlen(field_name) + 10);
    sprintf(field_find_text, "\"%s\":", field_name);
    char *start_str = strstr(data, field_find_text);
    if (start_str) {
       // printf("start str: %s\n", start_str);
        char *end_str = strpbrk(start_str, ",}");
        if (end_str) {
            // printf("end str: %s\n", end_str);
            int start_position = (start_str - data) + strlen(field_find_text);
            int end_position = (end_str - data);
            int length = end_position - start_position;
            // printf(data);
            // printf("%d %d %d\n", start_position, end_position, length);
            if ( length > 0) {
                char *work_str = strdup(&data[start_position]);
                start_str = work_str;
                if (start_str[0] == '"' && start_str[length - 1] == '"' && length >= 2) {
                    start_str ++;
                    length -= 2;
                }
                value_text = malloc(length + 2);
                strncpy(value_text, start_str, length);
                value_text[length] = 0;
                free(work_str);
            }
        }
    }
    free(field_find_text);
    return value_text;
}

const int convex_transaction_prepare(const convex_p convex, const char *transaction, const address_t address, char *hash, int hash_length) {
    response_t response;
    memset(&response, 0, sizeof(response_t));

    CURLU *url = convex_create_curl_url(convex, "/api/v1/transaction/prepare");
    if (!url) {
        return CONVEX_ERROR_INVALID_URL;
    }
    char * data = create_data_string(transaction, address, CONVEX_DEFAULT_LANGUAGE);
    if (!data) {
        curl_url_cleanup(url);
        return CONVEX_ERROR_INVALID_DATA;
    }
    int result = curl_request_post(url, data, &response);
    free(data);
    curl_url_cleanup(url);
    if (result == CONVEX_OK && response.code == 200) {
        char *value = get_value_from_json(response.data, "hash");
        if (value) {
            if (strlen(value) < hash_length) {
                strcpy(hash, value);
            }
            else {
                result = CONVEX_ERROR_BAD_HASH;
            }
            free(value);
        }
    }
    else {
        result = CONVEX_ERROR_PREPARE_FAILED;
    }
    response_clear(&response);
    return result;
}

const int convex_transaction_submit(
        const convex_p convex,
        const address_t address,
        const char *public_key,
        const char *hash_data,
        const char *signed_data
    ) {

    CURLU *url = convex_create_curl_url(convex, "/api/v1/transaction/submit");
    if (!url) {
        return CONVEX_ERROR_INVALID_URL;
    }
    char data[1024];
    sprintf(data, "{\"address\":\"#%ld\",\"accountKey\":\"%s\",\"hash\":\"%s\",\"sig\":\"%s\"}", address, public_key, hash_data, signed_data);

    int result = curl_request_post(url, data, &convex->response);
    curl_url_cleanup(url);
    if ( !(result == CONVEX_OK || convex->response.code == 200)) {
        result = CONVEX_ERROR_SUBMIT_FAILED;
    }
    return result;
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

/**
 * Create a new account with the convex network.
 *
 * @param[in] convex Convex handle with the convex netwok data.
 *
 * @param[in] account Convex account with a valid public/private key.
 *
 * @param[out] address Pointer to an address var that will contain the new address.
 *
 * @return CONVEX_OK if the create account was valid and the new address will be set.
 *
 */
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

    int result = curl_request_post(url, data, &response);
    curl_url_cleanup(url);

    if (result == CONVEX_OK && response.code == 200) {
        char *value = get_value_from_json(response.data, "address");
        if (value) {
            *address = atoi(value);
            free(value);
        }
        else {
            result = CONVEX_ERROR_NO_ACCOUNT_ADDRESS;
        }
    }
    else {
        result = CONVEX_ERROR_CREATE_ACCOUNT_FAILED;
    }
    response_clear(&response);
    return result;
}

const int convex_request_funds(convex_p convex, const amount_t amount, const address_t address, amount_t *result_amount) {
    response_t response;
    memset(&response, 0, sizeof(response_t));

    if (!convex) {
        return CONVEX_ERROR_INVALID_PARAMETER;
    }
    if (amount== 0) {
        return CONVEX_ERROR_INVALID_PARAMETER;
    }
    if (address == 0) {
        return CONVEX_ERROR_INVALID_PARAMETER;
    }

    CURLU *url = convex_create_curl_url(convex, "/api/v1/faucet");
    if (!url) {
        return CONVEX_ERROR_INVALID_URL;
    }

    char data[120];
    sprintf(data, "{\"address\": \"#%ld\", \"amount\": %ld}", address, amount);

    int result = curl_request_post(url, data, &response);
    curl_url_cleanup(url);
    if (result == CONVEX_OK && response.code == 200) {
        if (result_amount) {
            char *field_value = get_value_from_json(response.data, "value");
            if (field_value) {
                // printf("value: %s\n", field_value);
                *result_amount = atol(field_value);
                free(field_value);
            }
        }
    }
    else {
        result = CONVEX_ERROR_REQUEST_FUNDS_FAILED;
    }

    response_clear(&response);
    return result;
}

const int convex_get_address(const convex_p convex, const char *name, const address_t address, address_t *result_address) {
    if (result_address == NULL) {
        return CONVEX_ERROR_INVALID_PARAMETER;
    }
    // create a temp convex session for this request
    convex_p convex_work = convex_init(convex_get_url(convex));

    char *query_text = malloc(strlen(name) + 20);
    sprintf(query_text, "(address %s)", name);

    int result = convex_query(convex_work, query_text, address);
    if (result == CONVEX_OK) {
        char *value = get_value_from_json(convex_work->response.data, "value");
        if (value) {
            *result_address = atol(value);
            free(value);
        }
    }
    free(query_text);
    convex_close(convex_work);
    return result;
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
const int convex_query(const convex_p convex, const char *query, const address_t address) {

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

const int convex_send( const convex_p convex, const char *transaction, const convex_account_p account, const address_t address) {
    if (!convex) {
        return CONVEX_ERROR_INVALID_PARAMETER;
    }
    if (!transaction) {
        return CONVEX_ERROR_INVALID_PARAMETER;
    }
    if (!account) {
        return CONVEX_ERROR_INVALID_PARAMETER;
    }

    char hash_str[80];
    int hash_str_length = 80;
    int result = convex_transaction_prepare(convex, transaction, address, hash_str, hash_str_length);

    if (result != CONVEX_OK) {
        return result;
    }

    // convert hash_str to hash_data bytes
    char hash_data[32];
    int hash_data_length = 32;

    result = convex_utils_hex_to_bytes(hash_str, hash_data, &hash_data_length);
    if (result != CONVEX_OK) {
        return result;
    }

    // sign hash bytes with account key
    unsigned char signed_data[64];
    int signed_data_length = 64;
    result = convex_account_sign_data(account, hash_data, hash_data_length, signed_data, &signed_data_length);
    if (result != CONVEX_OK) {
        return result;
    }

    // convert signed bytes to signed hex
    int signed_hex_length = (64 * 2) + 1;
    char signed_hex[signed_hex_length];
    result = convex_utils_bytes_to_hex(signed_data, signed_data_length, signed_hex, &signed_hex_length);
    if (result != CONVEX_OK) {
        return result;
    }

    // call submit with address, public_key(hex), hash (hex), signed data(hex)
    const char *public_key = convex_account_get_public_key(account);
    result = convex_transaction_submit(convex, address, public_key, hash_str, signed_hex);

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
