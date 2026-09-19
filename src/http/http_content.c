/*
 * Project: C Web Engine
 * Author: Mohammad Ali Shahimian
 * Module: HTTP Content Validation.
 * Description: HTTP Content-Type validation utilities.
 */

#include "http_content.h"

#include <string.h>


int http_is_json_request(
    HttpRequest *request
) {

    if (request == NULL) {
        return 0;
    }


    const char *content_type =
        http_get_header(
            request,
            "Content-Type"
        );


    if (content_type == NULL) {
        return 0;
    }


    if (
        strcmp(
            content_type,
            "application/json"
        ) == 0
    ) {
        return 1;
    }


    return 0;
}
