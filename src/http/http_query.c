/*
 * Project: C Web Engine
 * Author: Mohammad Ali Shahimian
 * Description: HTTP query string parsing and lookup.
 */

#include "http_query.h"

#include <string.h>

static int hex_to_value(char c);

static void url_decode(
    char *destination,
    size_t destination_size,
    const char *source
);

void http_parse_query(
    HttpRequest *request
) {
    request->query[0] = '\0';
    request->query_param_count = 0;

    char *query_start = strchr(request->path, '?');

    if (query_start == NULL) {
        return;
    }

    *query_start = '\0';
    query_start++;

    strncpy(
        request->query,
        query_start,
        sizeof(request->query) - 1
    );

    request->query[sizeof(request->query) - 1] = '\0';

    if (request->query[0] == '\0') {
        return;
    }

    char query_copy[sizeof(request->query)];

    strncpy(
        query_copy,
        request->query,
        sizeof(query_copy) - 1
    );

    query_copy[sizeof(query_copy) - 1] = '\0';

    char *parameter = strtok(query_copy, "&");

    while (
        parameter != NULL &&
        request->query_param_count < MAX_QUERY_PARAMS
    ) {
        char *equals = strchr(parameter, '=');

        if (equals != NULL) {
            *equals = '\0';

            url_decode(
                request->query_params[request->query_param_count].name,
                sizeof(request->query_params[0].name),
                parameter
            );

            url_decode(
                request->query_params[request->query_param_count].value,
                sizeof(request->query_params[0].value),
                equals + 1
            );

            request->query_param_count++;
        }

        parameter = strtok(NULL, "&");
    }
}

const char *http_get_query_param(
    HttpRequest *request,
    const char *name
) {
    for (int i = 0; i < request->query_param_count; i++) {
        if (strcmp(request->query_params[i].name, name) == 0) {
            return request->query_params[i].value;
        }
    }

    return NULL;
}

static int hex_to_value(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    }

    if (c >= 'A' && c <= 'F') {
        return c - 'A' + 10;
    }

    if (c >= 'a' && c <= 'f') {
        return c - 'a' + 10;
    }

    return -1;
}

static void url_decode(
    char *destination,
    size_t destination_size,
    const char *source
) {
    size_t destination_index = 0;

    for (
        size_t source_index = 0;
        source[source_index] != '\0' &&
        destination_index < destination_size - 1;
        source_index++
    ) {
        if (
            source[source_index] == '%' &&
            source[source_index + 1] != '\0' &&
            source[source_index + 2] != '\0'
        ) {
            int high = hex_to_value(source[source_index + 1]);
            int low = hex_to_value(source[source_index + 2]);

            if (high >= 0 && low >= 0) {
                destination[destination_index++] =
                    (char)((high << 4) | low);

                source_index += 2;
                continue;
            }
        }

        if (source[source_index] == '+') {
            destination[destination_index++] = ' ';
            continue;
        }

        destination[destination_index++] = source[source_index];
    }

    destination[destination_index] = '\0';
}