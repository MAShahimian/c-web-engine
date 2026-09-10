/*
 * Project: C Web Engine
 * Author: Mohammad Ali Shahimian
 * Description: HTTP request parsing and response handling.
 */

#include "http.h"
#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <strings.h>

int http_parse_request(const char *buffer, HttpRequest *request) {

    request->header_count = 0;

    int parsed = sscanf(
        buffer,
        "%15s %255s %15s",
        request->method,
        request->path,
        request->version
    );

    if (parsed != 3) {
        return 0;
    }

    const char *line = strstr(buffer, "\r\n");

    if (line == NULL) {
        return 1;
    }

    line += 2;

    while (*line != '\0' && request->header_count < MAX_HEADERS) {
        if (line[0] == '\r' && line[1] == '\n') {
            break;
        }

        const char *line_end = strstr(line, "\r\n");

        if (line_end == NULL) {
            break;
        }

        const char *colon = strchr(line, ':');

        if (colon != NULL && colon < line_end) {
            size_t name_length = colon - line;

            if (name_length >= sizeof(request->headers[0].name)) {
                name_length = sizeof(request->headers[0].name) - 1;
            }

            memcpy(
                request->headers[request->header_count].name,
                line,
                name_length
            );

            request->headers[request->header_count].name[name_length] = '\0';

            const char *value_start = colon + 1;

            while (value_start < line_end && *value_start == ' ') {
                value_start++;
            }

            size_t value_length = line_end - value_start;

            if (value_length >= sizeof(request->headers[0].value)) {
                value_length = sizeof(request->headers[0].value) - 1;
            }

            memcpy(
                request->headers[request->header_count].value,
                value_start,
                value_length
            );

            request->headers[request->header_count].value[value_length] = '\0';

            request->header_count++;
        }

        line = line_end + 2;
    }

    return 1;
}

const char *http_get_header(
    HttpRequest *request,
    const char *name
) {
    for (int i = 0; i < request->header_count; i++) {
        if (strcasecmp(request->headers[i].name, name) == 0) {
            return request->headers[i].value;
        }
    }

    return NULL;
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
