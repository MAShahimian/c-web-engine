/*
 * Project: C Web Engine
 * Author: Mohammad Ali Shahimian
 * Module: HTTP JSON Parser
 * Description: Simple JSON request body parsing utilities.
 */

#ifndef HTTP_JSON_H
#define HTTP_JSON_H

#include <stddef.h>


/*
 * Extract a string value from a simple JSON object.
 *
 * Example:
 *
 * Input:
 * {
 *     "name":"Cash"
 * }
 *
 * Key:
 * name
 *
 * Output:
 * Cash
 *
 * Return:
 * 1  -> success
 * 0  -> key not found or invalid input
 */
int json_get_value(
    const char *json,
    const char *key,
    char *output,
    size_t output_size
);


#endif