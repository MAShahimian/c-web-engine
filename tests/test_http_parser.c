#include <stdio.h>
#include <string.h>

#include "../src/http/http.h"

int main(void) {
    HttpRequest request;

    const char *input =
        "GET /hello HTTP/1.1\r\n"
        "Host: localhost:8080\r\n"
        "User-Agent: curl/8.5.0\r\n"
        "Accept: */*\r\n"
        "Connection: close\r\n"
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

    if (request.header_count != 4) {
        printf("[FAIL] Header count mismatch: expected 4, got %d\n",
            request.header_count);
        return 1;
    }

    printf("[PASS] Parse multiple HTTP headers\n");

    if (strcmp(request.headers[0].name, "Host") != 0 ||
        strcmp(request.headers[0].value, "localhost:8080") != 0) {
        printf("[FAIL] Host header mismatch\n");
        return 1;
    }

    if (strcmp(request.headers[1].name, "User-Agent") != 0 ||
        strcmp(request.headers[1].value, "curl/8.5.0") != 0) {
        printf("[FAIL] User-Agent header mismatch\n");
        return 1;
    }

    if (strcmp(request.headers[2].name, "Accept") != 0 ||
        strcmp(request.headers[2].value, "*/*") != 0) {
        printf("[FAIL] Accept header mismatch\n");
        return 1;
    }

    if (strcmp(request.headers[3].name, "Connection") != 0 ||
        strcmp(request.headers[3].value, "close") != 0) {
        printf("[FAIL] Connection header mismatch\n");
        return 1;
    }

    printf("[PASS] Validate HTTP header names and values\n");

    const char *spacing_input =
        "GET /hello HTTP/1.1\r\n"
        "Host:    localhost\r\n"
        "Accept:   */*\r\n"
        "Connection:close\r\n"
        "\r\n";

    result = http_parse_request(spacing_input, &request);

    if (!result) {
        printf("[FAIL] Parser rejected headers with different spacing\n");
        return 1;
    }

    if (request.header_count != 3) {
        printf("[FAIL] Spacing test header count mismatch: expected 3, got %d\n",
            request.header_count);
        return 1;
    }

    if (strcmp(request.headers[0].name, "Host") != 0 ||
        strcmp(request.headers[0].value, "localhost") != 0) {
        printf("[FAIL] Spacing test Host header mismatch\n");
        return 1;
    }

    if (strcmp(request.headers[1].name, "Accept") != 0 ||
        strcmp(request.headers[1].value, "*/*") != 0) {
        printf("[FAIL] Spacing test Accept header mismatch\n");
        return 1;
    }

    if (strcmp(request.headers[2].name, "Connection") != 0 ||
        strcmp(request.headers[2].value, "close") != 0) {
        printf("[FAIL] Spacing test Connection header mismatch\n");
        return 1;
    }

    printf("[PASS] Parse headers with different spacing\n");    

    const char *host = http_get_header(
        &request,
        "Host"
    );

    if (host == NULL || strcmp(host, "localhost") != 0) {
        printf("[FAIL] Header lookup failed for Host\n");
        return 1;
    }

    printf("[PASS] Lookup HTTP header by name\n");

    const char *host_lowercase = http_get_header(
        &request,
        "host"
    );

    if (host_lowercase == NULL ||
        strcmp(host_lowercase, "localhost") != 0) {
        printf("[FAIL] Case-insensitive lookup failed for lowercase header name\n");
        return 1;
    }

    const char *host_uppercase = http_get_header(
        &request,
        "HOST"
    );

    if (host_uppercase == NULL ||
        strcmp(host_uppercase, "localhost") != 0) {
        printf("[FAIL] Case-insensitive lookup failed for uppercase header name\n");
        return 1;
    }

    printf("[PASS] Case-insensitive HTTP header lookup\n");

    const char *missing = http_get_header(
        &request,
        "Authorization"
    );

    if (missing != NULL) {
        printf("[FAIL] Missing header should return NULL\n");
        return 1;
    }

    printf("[PASS] Return NULL for missing HTTP header\n");    

    const char *invalid_input =
        "GET /hello\r\n";

    result = http_parse_request(invalid_input, &request);

    if (result) {
        printf("[FAIL] Parser accepted invalid request\n");
        return 1;
    }

    printf("[PASS] Reject invalid HTTP request\n");

    const char *invalid_header =
        "GET /hello HTTP/1.1\r\n"
        "Host localhost\r\n"
        "\r\n";

    result = http_parse_request(invalid_header, &request);

    if (result) {
        printf("[FAIL] Parser accepted header without colon\n");
        return 1;
    }

    printf("[PASS] Reject HTTP header without colon\n");

    const char *query_input =
        "GET /hello?name=Ali&age=30 HTTP/1.1\r\n"
        "Host: localhost:8080\r\n"
        "\r\n";

    result = http_parse_request(query_input, &request);

    if (!result) {
        printf("[FAIL] Parser rejected request with query string\n");
        return 1;
    }

    if (strcmp(request.path, "/hello") != 0) {
        printf("[FAIL] Query request path mismatch\n");
        return 1;
    }

    if (strcmp(request.query, "name=Ali&age=30") != 0) {
        printf("[FAIL] Query string mismatch\n");
        return 1;
    }

    printf("[PASS] Parse HTTP query string\n");

    const char *query_params_input =
        "GET /hello?name=Ali&age=30 HTTP/1.1\r\n"
        "Host: localhost:8080\r\n"
        "\r\n";

    result = http_parse_request(query_params_input, &request);

    if (!result) {
        printf("[FAIL] Parser rejected query parameters\n");
        return 1;
    }

    if (request.query_param_count != 2) {
        printf(
            "[FAIL] Query parameter count mismatch: expected 2, got %d\n",
            request.query_param_count
        );
        return 1;
    }

    if (strcmp(request.query_params[0].name, "name") != 0 ||
        strcmp(request.query_params[0].value, "Ali") != 0) {
        printf("[FAIL] First query parameter mismatch\n");
        return 1;
    }

    if (strcmp(request.query_params[1].name, "age") != 0 ||
        strcmp(request.query_params[1].value, "30") != 0) {
        printf("[FAIL] Second query parameter mismatch\n");
        return 1;
    }

    printf("[PASS] Parse query parameters\n");    

    return 0;
}