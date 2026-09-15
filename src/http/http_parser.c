/*
 * Project: C Web Engine
 * Author: Mohammad Ali Shahimian
 * Description: HTTP request line and header parsing.
 */

#include "http_parser.h"
#include "http_query.h"

#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>

static int validate_content_length(
    HttpRequest *request,
    size_t actual_body_length
);

int http_parse_request(
    const char *buffer,
    HttpRequest *request
) {
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

    http_parse_query(request);

    const char *line = strstr(buffer, "\r\n");

    if (line == NULL) {
        return 1;
    }

    line += 2;

    while (
        *line != '\0' &&
        request->header_count < MAX_HEADERS
    ) {
        if (line[0] == '\r' && line[1] == '\n') {
            break;
        }

        const char *line_end = strstr(line, "\r\n");

        if (line_end == NULL) {
            break;
        }

        const char *colon = strchr(line, ':');

        if (colon == NULL || colon >= line_end) {
            return 0;
        }

        if (colon == line) {
            return 0;
        }

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

        while (
            value_start < line_end &&
            *value_start == ' '
        ) {
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

        request->headers[request->header_count]
            .value[value_length] = '\0';

        request->header_count++;

        line = line_end + 2;
    }

    request->body_length = 0;
    request->body[0] = '\0';

    const char *body_start = strstr(buffer, "\r\n\r\n");

    if (body_start != NULL) {
        body_start += 4;

        size_t body_length = strlen(body_start);

        if (body_length >= MAX_BODY_SIZE) {
            body_length = MAX_BODY_SIZE - 1;
        }

        if (!validate_content_length(request, body_length)) {
            return 0;
        }

        memcpy(
            request->body,
            body_start,
            body_length
        );

        request->body[body_length] = '\0';
        request->body_length = body_length;
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

static int validate_content_length(
    HttpRequest *request,
    size_t actual_body_length
) {
    const char *content_length =
        http_get_header(request, "Content-Length");

    if (content_length == NULL) {
        return 1;
    }

    char *end_pointer;

    long expected_length = strtol(
        content_length,
        &end_pointer,
        10
    );

    if (
        content_length == end_pointer ||
        *end_pointer != '\0' ||
        expected_length < 0 ||
        (size_t)expected_length >= MAX_BODY_SIZE
    ) {
        return 0;
    }

    return (size_t)expected_length == actual_body_length;
}
