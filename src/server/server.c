/*
 * Project: C Web Engine
 * Author: Mohammad Ali Shahimian
 * Description: Lightweight HTTP server built from scratch in C.
 */

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

#include "../http/http.h"
#include "../router/router.h"

int main(void) {
    int server_socket;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket == -1) {
        perror("socket");
        return 1;
    }

    int opt = 1;

    setsockopt(
        server_socket,
        SOL_SOCKET,
        SO_REUSEADDR,
        &opt,
        sizeof(opt)
    );

    printf("Socket FD: %d\n", server_socket);

    struct sockaddr_in server_address;

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(8080);

    int result = bind(
        server_socket,
        (struct sockaddr *)&server_address,
        sizeof(server_address)
    );

    if (result == -1) {
        perror("bind");
        close(server_socket);
        return 1;
    }

    printf("Server bound to port 8080\n");

    result = listen(server_socket, 5);

    if (result == -1) {
        perror("listen");
        close(server_socket);
        return 1;
    }

    printf("Server is listening on port 8080\n");

    while (1) {
        int client_socket = accept(
            server_socket,
            NULL,
            NULL
        );

        if (client_socket == -1) {
            perror("accept");
            continue;
        }

        printf("Client connected!\n");

        char buffer[1024];

        int bytes_received = recv(
            client_socket,
            buffer,
            sizeof(buffer) - 1,
            0
        );

        if (bytes_received == -1) {
            perror("recv");
            close(client_socket);
            continue;
        }

        if (bytes_received == 0) {
            close(client_socket);
            continue;
        }

        buffer[bytes_received] = '\0';

        HttpRequest request;

        if (http_parse_request(buffer, &request)) {
            router_handle_request(client_socket, &request);
        } else {
            http_send_text_response(
                client_socket,
                400,
                "Bad Request",
                "Bad Request\n"
            );
        }

        close(client_socket);
    }

    close(server_socket);

    return 0;
}