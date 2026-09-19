/*
 * Project: C Web Engine
 * Author: Mohammad Ali Shahimian
 * Module: HTTP JSON Parser
 * Description: Simple JSON object value extraction.
 */

#include "http_json.h"

#include <string.h>
#include <stdio.h>


int json_get_value(
    const char *json,
    const char *key,
    char *output,
    size_t output_size
) {

    if (
        json == NULL ||
        key == NULL ||
        output == NULL ||
        output_size == 0
    ) {
        return 0;
    }


    char pattern[128];


    snprintf(
        pattern,
        sizeof(pattern),
        "\"%s\"",
        key
    );


    const char *key_position = strstr(
        json,
        pattern
    );


    if (key_position == NULL) {
        return 0;
    }


    const char *colon = strchr(
        key_position,
        ':'
    );


    if (colon == NULL) {
        return 0;
    }


    const char *value_start = colon + 1;


    while (
        *value_start == ' ' ||
        *value_start == '\t'
    ) {
        value_start++;
    }


    if (*value_start != '"') {
        return 0;
    }


    value_start++;


    const char *value_end = strchr(
        value_start,
        '"'
    );


    if (value_end == NULL) {
        return 0;
    }


    size_t length = value_end - value_start;


    if (length >= output_size) {
        length = output_size - 1;
    }


    memcpy(
        output,
        value_start,
        length
    );


    output[length] = '\0';


    return 1;
}