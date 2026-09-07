/*
 * Project: C Web Engine
 * Author: Mohammad Ali Shahimian
 * Module: HTTP Router
 * Description: Handles HTTP request routing and dispatches requests to handlers.
 */
 
#include "router.h"

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

static void handle_hello(int client_socket) {
    http_send_response(client_socket);
}

static void handle_about(int client_socket) {
    const char *body = "C Web Engine\n";

    char response[1024];

    int response_length = snprintf(
        response,
        sizeof(response),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: %zu\r\n"
        "Connection: close\r\n"
        "\r\n"
        "%s",
        strlen(body),
        body
    );

    send(
        client_socket,
        response,
        response_length,
        0
    );
}

typedef struct {
    const char *path;
    HttpHandler handler;
} Route;

static Route routes[] = {
    {"/hello", handle_hello},
    {"/about", handle_about}
};

void router_handle_request(int client_socket, HttpRequest *request) {
    int route_count = sizeof(routes) / sizeof(routes[0]);

    for (int i = 0; i < route_count; i++) {
        if (strcmp(request->path, routes[i].path) == 0) {
            routes[i].handler(client_socket);
            return;
        }
    }

    const char *body = "Not Found\n";

    char response[1024];

    int response_length = snprintf(
        response,
        sizeof(response),
        "HTTP/1.1 404 Not Found\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: %zu\r\n"
        "Connection: close\r\n"
        "\r\n"
        "%s",
        strlen(body),
        body
    );

    send(
        client_socket,
        response,
        response_length,
        0
    );
}