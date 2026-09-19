#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include <stddef.h>
#include "http.h"

void http_send_text_response(
    int client_socket,
    int status_code,
    const char *status_text,
    const char *body
);

int http_send_json_response(
    char *buffer,
    size_t buffer_size,
    int status_code,
    const char *json_body
);

#endif