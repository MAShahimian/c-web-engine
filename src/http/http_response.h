#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include "http.h"

void http_send_text_response(
    int client_socket,
    int status_code,
    const char *status_text,
    const char *body
);

#endif