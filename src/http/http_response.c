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

int http_send_json_response(
    char *buffer,
    size_t buffer_size,
    int status_code,
    const char *json_body
) {

    int length = snprintf(
        buffer,
        buffer_size,
        "HTTP/1.1 %d OK\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: %zu\r\n"
        "Connection: close\r\n"
        "\r\n"
        "%s",
        status_code,
        strlen(json_body),
        json_body
    );


    return length;
}