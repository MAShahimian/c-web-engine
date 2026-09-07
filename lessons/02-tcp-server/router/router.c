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

    http_send_text_response(
        client_socket,
        404,
        "Not Found",
        "Not Found\n"
    );
}