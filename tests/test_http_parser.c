#include <stdio.h>
#include <string.h>

#include "../src/http/http.h"

int main(void) {
    HttpRequest request;

    const char *input =
        "GET /hello HTTP/1.1\r\n"
        "Host: localhost\r\n"
        "\r\n";

    int result = http_parse_request(input, &request);

    if (!result) {
        printf("[FAIL] Parser rejected valid request\n");
        return 1;
    }

    if (strcmp(request.method, "GET") != 0) {
        printf("[FAIL] Method mismatch\n");
        return 1;
    }

    if (strcmp(request.path, "/hello") != 0) {
        printf("[FAIL] Path mismatch\n");
        return 1;
    }

    if (strcmp(request.version, "HTTP/1.1") != 0) {
        printf("[FAIL] Version mismatch\n");
        return 1;
    }

    printf("[PASS] Parse GET /hello HTTP/1.1\n");

    if (request.header_count != 1) {
        printf("[FAIL] Header count mismatch\n");
        return 1;
    }

    if (strcmp(request.headers[0].name, "Host") != 0) {
        printf("[FAIL] Header name mismatch\n");
        return 1;
    }

    if (strcmp(request.headers[0].value, "localhost") != 0) {
        printf("[FAIL] Header value mismatch\n");
        return 1;
    }

    printf("[PASS] Parse HTTP headers\n");

    const char *invalid_input =
        "GET /hello\r\n";

    result = http_parse_request(invalid_input, &request);

    if (result) {
        printf("[FAIL] Parser accepted invalid request\n");
        return 1;
    }

    printf("[PASS] Reject invalid HTTP request\n");

    return 0;
}