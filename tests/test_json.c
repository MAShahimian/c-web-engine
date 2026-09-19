/*
 * Project: C Web Engine
 * Author: Mohammad Ali Shahimian
 * Description: JSON request parsing tests.
 */

#include <stdio.h>
#include <string.h>

#include "../src/http/http_json.h"


int main(void)
{
    const char *json =
        "{"
        "\"name\":\"Cash\","
        "\"type\":\"asset\""
        "}";


    char value[128];


    int result = json_get_value(
        json,
        "name",
        value,
        sizeof(value)
    );


    if (!result) {

        printf(
            "[FAIL] Cannot read JSON value\n"
        );

        return 1;
    }


    if (
        strcmp(
            value,
            "Cash"
        ) != 0
    ) {

        printf(
            "[FAIL] JSON value mismatch\n"
        );

        return 1;
    }


    printf(
        "[PASS] Parse JSON object value\n"
    );


    return 0;
}