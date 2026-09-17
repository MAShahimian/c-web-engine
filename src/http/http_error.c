/*
 * Project: C Web Engine
 * Author: Mohammad Ali Shahimian
 * Description: HTTP error response handling.
 */

#include "http_error.h"

#include <stdio.h>
#include <string.h>


int http_send_error_response(
    char *buffer,
    size_t buffer_size,
    int status_code,
    const char *status_text
) {

    const char *body =
        status_text;


    int length = snprintf(
        buffer,
        buffer_size,

        "HTTP/1.1 %d %s\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: %zu\r\n"
        "Connection: close\r\n"
        "\r\n"
        "%s",

        status_code,
        status_text,
        strlen(body),
        body
    );


    return length;
}