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

void http_send_response(int client_socket) {
    const char *body = "Hello from C Server!\n";

    char response[1024];

    int response_length = snprintf(
        response,
        sizeof(response),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: %zu\r\n"
        "Connection: close\r\n"
        "\r\n"
        "%s",
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
