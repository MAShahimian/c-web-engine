/*
 * Project: C Web Engine
 * Author: Mohammad Ali Shahimian
 * Description: HTTP error response handling.
 */

#ifndef HTTP_ERROR_H
#define HTTP_ERROR_H


#include <stddef.h>


int http_send_error_response(
    char *buffer,
    size_t buffer_size,
    int status_code,
    const char *status_text
);


#endif