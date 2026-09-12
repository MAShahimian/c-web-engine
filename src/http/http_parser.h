#ifndef HTTP_PARSER_H
#define HTTP_PARSER_H

#include "http.h"

int http_parse_request(
    const char *buffer,
    HttpRequest *request
);

#endif