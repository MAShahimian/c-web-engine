/*
 * Project: C Web Engine
 * Author: Mohammad Ali Shahimian
 * Description: HTTP query string parsing and lookup.
 */

#include "http_query.h"

#include <string.h>

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

            strncpy(
                request->query_params[request->query_param_count].name,
                parameter,
                sizeof(request->query_params[0].name) - 1
            );

            request->query_params[request->query_param_count]
                .name[sizeof(request->query_params[0].name) - 1] = '\0';

            strncpy(
                request->query_params[request->query_param_count].value,
                equals + 1,
                sizeof(request->query_params[0].value) - 1
            );

            request->query_params[request->query_param_count]
                .value[sizeof(request->query_params[0].value) - 1] = '\0';

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