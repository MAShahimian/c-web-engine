/*
 * Project: C Web Engine
 * Author: Mohammad Ali Shahimian
 * Description: HTTP response generation tests.
 */

#include <stdio.h>
#include <string.h>

#include "../src/http/http_response.h"


int main(void)
{
    char response_buffer[1024];


    int length = http_send_json_response(
        response_buffer,
        sizeof(response_buffer),
        200,
        "{\"name\":\"Cash\"}"
    );


    if (length <= 0) {

        printf(
            "[FAIL] Failed to generate JSON response\n"
        );

        return 1;
    }


    if (
        strstr(
            response_buffer,
            "HTTP/1.1 200 OK"
        ) == NULL
    ) {

        printf(
            "[FAIL] Missing HTTP status\n"
        );

        return 1;
    }


    if (
        strstr(
            response_buffer,
            "Content-Type: application/json"
        ) == NULL
    ) {

        printf(
            "[FAIL] Missing JSON content type\n"
        );

        return 1;
    }


    if (
        strstr(
            response_buffer,
            "{\"name\":\"Cash\"}"
        ) == NULL
    ) {

        printf(
            "[FAIL] JSON body mismatch\n"
        );

        return 1;
    }


    printf(
        "[PASS] Generate JSON HTTP response\n"
    );


    return 0;
}