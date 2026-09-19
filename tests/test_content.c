/*
 * Project: C Web Engine
 * Author: Mohammad Ali Shahimian
 * Description: HTTP Content-Type validation tests.
 */

#include <stdio.h>
#include <string.h>

#include "../src/http/http_content.h"
#include "../src/http/http_parser.h"


int main(void)
{
    HttpRequest request;


    strcpy(
        request.headers[0].name,
        "Content-Type"
    );

    strcpy(
        request.headers[0].value,
        "application/json"
    );

    request.header_count = 1;


    if (
        !http_is_json_request(&request)
    ) {

        printf(
            "[FAIL] JSON Content-Type validation failed\n"
        );

        return 1;
    }


    printf(
        "[PASS] Validate application/json Content-Type\n"
    );


    strcpy(
        request.headers[0].value,
        "text/plain"
    );


    if (
        http_is_json_request(&request)
    ) {

        printf(
            "[FAIL] Invalid JSON Content-Type accepted\n"
        );

        return 1;
    }


    printf(
        "[PASS] Reject non JSON Content-Type\n"
    );


    return 0;
}