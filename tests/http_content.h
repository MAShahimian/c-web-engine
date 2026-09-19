/*
 * Project: C Web Engine
 * Author: Mohammad Ali Shahimian
 * Module: HTTP Content Validation.
 */

#ifndef HTTP_CONTENT_H
#define HTTP_CONTENT_H


#include "http_parser.h"


int http_is_json_request(
    HttpRequest *request
);


#endif