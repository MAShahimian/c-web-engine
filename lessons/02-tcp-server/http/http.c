#include "http.h"
#include <stdio.h>

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