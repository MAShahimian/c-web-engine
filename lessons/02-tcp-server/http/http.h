/*
 * Project: C Web Engine
 * Author: Mohammad Ali Shahimian
 * Description: HTTP data structures and public API.
 */

#ifndef HTTP_H
#define HTTP_H

typedef struct {
    char method[16];
    char path[256];
    char version[16];
} HttpRequest;

typedef void (*HttpHandler)(int client_socket);

int http_parse_request(const char *buffer, HttpRequest *request);

void http_send_response(int client_socket);

#endif