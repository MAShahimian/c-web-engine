/*
 * Project: C Web Engine
 * Author: Mohammad Ali Shahimian
 * Module: HTTP Router
 * Description: Defines the public API for HTTP request routing.
 */
 
#ifndef ROUTER_H
#define ROUTER_H

#include "../http/http.h"

void router_handle_request(int client_socket, HttpRequest *request);

#endif