/*
 * Project: C Web Engine
 * Author: Mohammad Ali Shahimian
 * Description: HTTP request parsing and response handling.
 */

#include "http.h"
#include <stdio.h>
#include <sys/socket.h>
#include <string.h>

int http_parse_request(const char *buffer, HttpRequest *request) {
    int parsed = sscanf(
        buffer,
        "%15s %255s %15s",
        request->method,
        request->path,
        request->version
    );

    return parsed == 3;
}

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
