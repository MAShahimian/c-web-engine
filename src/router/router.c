/*
 * Project: C Web Engine
 * Author: Mohammad Ali Shahimian
 * Module: HTTP Router
 * Description: Handles HTTP request routing and dispatches requests to handlers.
 */

#include "router.h"

#include <string.h>

static void handle_hello(int client_socket) {
    http_send_text_response(
        client_socket,
        200,
        "OK",
        "Hello from C Server!\n"
    );
}

static void handle_about(int client_socket) {
    http_send_text_response(
        client_socket,
        200,
        "OK",
        "C Web Engine\n"
    );
}

static void handle_health(int client_socket) {
    http_send_text_response(
        client_socket,
        200,
        "OK",
        "OK\n"
    );
}

typedef struct {
    const char *method;
    const char *path;
    HttpHandler handler;
} Route;

static Route routes[] = {
    {"GET", "/hello", handle_hello},
    {"GET", "/about", handle_about},
    {"GET", "/health", handle_health}
};

void router_handle_request(int client_socket, HttpRequest *request) {
    int route_count = sizeof(routes) / sizeof(routes[0]);
    int path_found = 0;

    for (int i = 0; i < route_count; i++) {
        if (strcmp(request->path, routes[i].path) == 0) {
            path_found = 1;

            if (strcmp(request->method, routes[i].method) == 0) {
                routes[i].handler(client_socket);
                return;
            }
        }
    }

    if (path_found) {
        http_send_text_response(
            client_socket,
            405,
            "Method Not Allowed",
            "Method Not Allowed\n"
        );

        return;
    }

    http_send_text_response(
        client_socket,
        404,
        "Not Found",
        "Not Found\n"
    );
}
