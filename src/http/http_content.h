/*
 * Project: C Web Engine
 * Author: Mohammad Ali Shahimian
 * Module: HTTP Content Validation.
 * Description: HTTP Content-Type validation interface.
 */

#ifndef HTTP_CONTENT_H
#define HTTP_CONTENT_H

#include "http_parser.h"


/*
 * Check whether the HTTP request contains
 * Content-Type: application/json
 *
 * Return:
 * 1 -> JSON request
 * 0 -> Not JSON request
 */
int http_is_json_request(
    HttpRequest *request
);


#endif