/*
 * Project: C Web Engine
 * Author: Mohammad Ali Shahimian
 * Description: HTTP data structures and public API.
 */

#ifndef HTTP_H
#define HTTP_H

#define MAX_HEADERS 16

typedef struct {
    char name[64];
    char value[256];
} HttpHeader;

typedef struct {
    char method[16];
    char path[256];
    char version[16];
    HttpHeader headers[MAX_HEADERS];
    int header_count;
} HttpRequest;

typedef void (*HttpHandler)(int client_socket);

int http_parse_request(const char *buffer, HttpRequest *request);

typedef struct {
    int status_code;
    const char *status_text;
    const char *content_type;
    const char *body;
} HttpResponse;

void http_send_response(
    int client_socket,
    HttpResponse *response
);

void http_send_text_response(
    int client_socket,
    int status_code,
    const char *status_text,
    const char *body
);

#endif