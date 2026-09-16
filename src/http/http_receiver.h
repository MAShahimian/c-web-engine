#ifndef HTTP_RECEIVER_H
#define HTTP_RECEIVER_H

#include <stddef.h>

int http_receive_request(
    int client_socket,
    char *buffer,
    size_t buffer_size
);

#endif