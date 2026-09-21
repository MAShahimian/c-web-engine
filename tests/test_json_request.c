/*
 * Project: C Web Engine
 * Author: Mohammad Ali Shahimian
 * Description: Full HTTP JSON request integration test.
 */


#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>


#include "../src/http/http_receiver.h"
#include "../src/http/http_parser.h"
#include "../src/http/http_content.h"
#include "../src/http/http_json.h"



int main(void)
{
    int sockets[2];


    if (
        socketpair(
            AF_UNIX,
            SOCK_STREAM,
            0,
            sockets
        ) == -1
    ) {

        printf(
            "[FAIL] Create socket pair\n"
        );

        return 1;
    }


    const char *json_body =
        "{\"name\":\"Ali\",\"amount\":1000}";


    char request[512];


    snprintf(
        request,
        sizeof(request),

        "POST /accounts HTTP/1.1\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: %zu\r\n"
        "\r\n"
        "%s",

        strlen(json_body),
        json_body
    );


    send(
        sockets[0],
        request,
        strlen(request),
        0
    );


    char buffer[1024];


    int received =
        http_receive_request(
            sockets[1],
            buffer,
            sizeof(buffer)
        );


    if (received <= 0) {

        printf(
            "[FAIL] Receive HTTP request\n"
        );

        return 1;
    }


    HttpRequest request_data;


    if (
        !http_parse_request(
            buffer,
            &request_data
        )
    ) {

        printf(
            "[FAIL] Parse HTTP JSON request\n"
        );

        return 1;
    }


    printf(
        "[PASS] Parse HTTP JSON request\n"
    );



    if (
        !http_is_json_request(
            &request_data
        )
    ) {

        printf(
            "[FAIL] Validate JSON Content-Type\n"
        );

        return 1;
    }


    printf(
        "[PASS] Validate JSON Content-Type\n"
    );



    char value[64];


    if (
        !json_get_value(
            request_data.body,
            "name",
            value,
            sizeof(value)
        )
    ) {

        printf(
            "[FAIL] Extract JSON body value\n"
        );

        return 1;
    }


    if (
        strcmp(
            value,
            "Ali"
        ) != 0
    ) {

        printf(
            "[FAIL] JSON value mismatch\n"
        );

        return 1;
    }


    printf(
        "[PASS] Extract JSON body value\n"
    );


    close(sockets[0]);
    close(sockets[1]);


    return 0;
}