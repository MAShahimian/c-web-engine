/*
 * Project: C Web Engine
 * Author: Mohammad Ali Shahimian
 * Description: Receive complete HTTP requests from TCP clients.
 */

#include "http_receiver.h"

#include <string.h>
#include <sys/socket.h>


static int find_header_end(
    const char *buffer,
    size_t length
);


int http_receive_request(
    int client_socket,
    char *buffer,
    size_t buffer_size
) {
    size_t total_received = 0;

    while (total_received < buffer_size - 1) {

        int bytes_received = recv(
            client_socket,
            buffer + total_received,
            buffer_size - total_received - 1,
            0
        );

        if (bytes_received <= 0) {
            return -1;
        }

        total_received += bytes_received;

        buffer[total_received] = '\0';


        int header_end = find_header_end(
            buffer,
            total_received
        );


        if (header_end != -1) {

            /*
             * Header is complete.
             * Body validation will be added here.
             */

            return (int)total_received;
        }
    }

    return -1;
}


static int find_header_end(
    const char *buffer,
    size_t length
) {

    for (size_t i = 0; i + 3 < length; i++) {

        if (
            buffer[i] == '\r' &&
            buffer[i + 1] == '\n' &&
            buffer[i + 2] == '\r' &&
            buffer[i + 3] == '\n'
        ) {
            return (int)(i + 4);
        }
    }

    return -1;
}
