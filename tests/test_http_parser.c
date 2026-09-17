#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "../src/http/http_receiver.h"
#include "../src/http/http_error.h"

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

    const char *name = http_get_query_param(
        &request,
        "name"
    );

    if (name == NULL || strcmp(name, "Ali") != 0) {
        printf("[FAIL] Query parameter lookup failed\n");
        return 1;
    }

    printf("[PASS] Lookup query parameter by name\n");

    const char *missing_param = http_get_query_param(
        &request,
        "city"
    );

    if (missing_param != NULL) {
        printf("[FAIL] Missing query parameter should return NULL\n");
        return 1;
    }

    printf("[PASS] Return NULL for missing query parameter\n");

    const char *encoded_query_input =
        "GET /hello?name=Mohammad%20Ali&city=Shiraz HTTP/1.1\r\n"
        "Host: localhost:8080\r\n"
        "\r\n";

    result = http_parse_request(encoded_query_input, &request);

    if (!result) {
        printf("[FAIL] Parser rejected URL-encoded query\n");
        return 1;
    }

    const char *encoded_name = http_get_query_param(
        &request,
        "name"
    );

    if (encoded_name == NULL ||
        strcmp(encoded_name, "Mohammad Ali") != 0) {
        printf("[FAIL] URL-decoded query parameter mismatch\n");
        return 1;
    }

    printf("[PASS] Decode URL-encoded query parameter\n");

    const char *body_input =
        "POST /users HTTP/1.1\r\n"
        "Host: localhost:8080\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 5\r\n"
        "\r\n"
        "Hello";

    result = http_parse_request(body_input, &request);

    if (!result) {
        printf("[FAIL] Parser rejected request with body\n");
        return 1;
    }

    if (request.body_length != 5) {
        printf("[FAIL] HTTP body length mismatch\n");
        return 1;
    }

    if (strcmp(request.body, "Hello") != 0) {
        printf("[FAIL] HTTP body content mismatch\n");
        return 1;
    }

    printf("[PASS] Parse HTTP request body\n");

    const char *valid_content_length_input =
        "POST /users HTTP/1.1\r\n"
        "Host: localhost:8080\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 5\r\n"
        "\r\n"
        "Hello";

    result = http_parse_request(
        valid_content_length_input,
        &request
    );

    if (!result) {
        printf("[FAIL] Valid Content-Length was rejected\n");
        return 1;
    }

    if (request.body_length != 5) {
        printf("[FAIL] Valid Content-Length body length mismatch\n");
        return 1;
    }

    printf("[PASS] Validate matching Content-Length\n");

    const char *invalid_content_length_input =
        "POST /users HTTP/1.1\r\n"
        "Host: localhost:8080\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 10\r\n"
        "\r\n"
        "Hello";

    result = http_parse_request(
        invalid_content_length_input,
        &request
    );

    if (result) {
        printf("[FAIL] Mismatched Content-Length was accepted\n");
        return 1;
    }

    printf("[PASS] Reject mismatched Content-Length\n");

    /*
    * Test receiving a complete HTTP request body.
    *
    * The request is intentionally split into multiple chunks
    * to simulate multiple TCP recv() calls.
    */

    const char *request_part_1 =
        "POST /users HTTP/1.1\r\n"
        "Host: localhost:8080\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 11\r\n"
        "\r\n"
        "Hello";

    const char *request_part_2 =
        " World";

    char complete_request[1024];

    size_t part_1_length = strlen(request_part_1);
    size_t part_2_length = strlen(request_part_2);

    memcpy(
        complete_request,
        request_part_1,
        part_1_length
    );

    memcpy(
        complete_request + part_1_length,
        request_part_2,
        part_2_length
    );

    complete_request[
        part_1_length + part_2_length
    ] = '\0';

    result = http_parse_request(
        complete_request,
        &request
    );

    if (!result) {
        printf("[FAIL] Parser rejected request received in multiple chunks\n");
        return 1;
    }

    if (request.body_length != 11) {
        printf("[FAIL] Complete body length mismatch\n");
        return 1;
    }

    if (strcmp(request.body, "Hello World") != 0) {
        printf("[FAIL] Complete body content mismatch\n");
        return 1;
    }

    printf("[PASS] Parse complete body received in multiple chunks\n");

    /*
    * Test that an incomplete request body is not treated
    * as a complete HTTP request.
    */

    const char *incomplete_request =
        "POST /users HTTP/1.1\r\n"
        "Host: localhost:8080\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 11\r\n"
        "\r\n"
        "Hello";

    result = http_parse_request(
        incomplete_request,
        &request
    );

    if (result) {
        printf("[FAIL] Incomplete HTTP body was accepted as complete\n");
        return 1;
    }

    printf("[PASS] Reject incomplete HTTP body\n");

    /*
    * Test receiving an HTTP request from multiple TCP chunks.
    */

    int sockets[2];

    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sockets) == -1) {
        perror("socketpair");
        return 1;
    }

    const char *socket_request_part_1 =
        "POST /users HTTP/1.1\r\n"
        "Host: localhost:8080\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 11\r\n"
        "\r\n"
        "Hello";

    const char *socket_request_part_2 =
        " World";

    if (send(
            sockets[0],
            socket_request_part_1,
            strlen(socket_request_part_1),
            0
        ) == -1) {
        perror("send");
        close(sockets[0]);
        close(sockets[1]);
        return 1;
    }

    if (send(
            sockets[0],
            socket_request_part_2,
            strlen(socket_request_part_2),
            0
        ) == -1) {
        perror("send");
        close(sockets[0]);
        close(sockets[1]);
        return 1;
    }

    char receive_buffer[1024];

    int received_length = http_receive_request(
        sockets[1],
        receive_buffer,
        sizeof(receive_buffer)
    );

    if (received_length == -1) {
        printf("[FAIL] Failed to receive complete HTTP request\n");
        close(sockets[0]);
        close(sockets[1]);
        return 1;
    }

    receive_buffer[received_length] = '\0';

    if (strcmp(
            receive_buffer,
            "POST /users HTTP/1.1\r\n"
            "Host: localhost:8080\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: 11\r\n"
            "\r\n"
            "Hello World"
        ) != 0) {

        printf("[FAIL] Received HTTP request does not match expected data\n");

        close(sockets[0]);
        close(sockets[1]);
        return 1;
    }

    printf("[PASS] Receive complete HTTP request from multiple TCP chunks\n");

    close(sockets[0]);
    close(sockets[1]);

    /*
    * Test receiving HTTP request until Content-Length body is complete.
    */

    int body_sockets[2];

    if (socketpair(
            AF_UNIX,
            SOCK_STREAM,
            0,
            body_sockets
        ) == -1) {

        perror("socketpair");
        return 1;
    }


    const char *body_request_header =
        "POST /message HTTP/1.1\r\n"
        "Host: localhost\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 11\r\n"
        "\r\n";


    const char *body_part_1 =
        "Hello";


    const char *body_part_2 =
        " World";


    send(
        body_sockets[0],
        body_request_header,
        strlen(body_request_header),
        0
    );


    send(
        body_sockets[0],
        body_part_1,
        strlen(body_part_1),
        0
    );


    send(
        body_sockets[0],
        body_part_2,
        strlen(body_part_2),
        0
    );


    char body_receive_buffer[1024];


    int body_received_length = http_receive_request(
        body_sockets[1],
        body_receive_buffer,
        sizeof(body_receive_buffer)
    );


    if (body_received_length <= 0) {

        printf(
            "[FAIL] Failed to receive request body\n"
        );

        close(body_sockets[0]);
        close(body_sockets[1]);

        return 1;
    }


    body_receive_buffer[body_received_length] = '\0';


    if (strcmp(
            body_receive_buffer,
            "POST /message HTTP/1.1\r\n"
            "Host: localhost\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: 11\r\n"
            "\r\n"
            "Hello World"
        ) != 0) {


        printf(
            "[FAIL] Received request body mismatch\n"
        );

        close(body_sockets[0]);
        close(body_sockets[1]);

        return 1;
    }


    printf(
        "[PASS] Receive body according to Content-Length\n"
    );


    close(body_sockets[0]);
    close(body_sockets[1]);

    /*
    * Test generating HTTP 400 Bad Request response.
    */

    char error_buffer[1024];


    int response_length = http_send_error_response(
        error_buffer,
        sizeof(error_buffer),
        400,
        "Bad Request"
    );


    if (response_length <= 0) {

        printf(
            "[FAIL] Failed to generate error response\n"
        );

        return 1;
    }


    if (
        strstr(
            error_buffer,
            "HTTP/1.1 400 Bad Request"
        ) == NULL
    ) {

        printf(
            "[FAIL] Invalid HTTP error status\n"
        );

        return 1;
    }


    printf(
        "[PASS] Generate HTTP 400 response\n"
    );

    return 0;
}
