#ifndef HTTP_QUERY_H
#define HTTP_QUERY_H

#include "http.h"

void http_parse_query(
    HttpRequest *request
);

const char *http_get_query_param(
    HttpRequest *request,
    const char *name
);

#endif