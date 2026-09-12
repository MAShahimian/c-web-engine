/*
 * Project: C Web Engine
 * Author: Mohammad Ali Shahimian
 * Description: HTTP response handling.
 */

#include "http_response.h"

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

void http_send_text_response(
    int client_socket,
    int status_code,
    const char *status_text,
    const char *body
) {
    char response[1024];

    int response_length = snprintf(
        response,
        sizeof(response),
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

    send(
        client_socket,
        response,
        response_length,
        0
    );
}