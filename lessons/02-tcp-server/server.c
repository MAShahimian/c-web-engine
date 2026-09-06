#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main(void) {
    int server_socket;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket == -1) {
        perror("socket");
        return 1;
    }

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

    close(server_socket);

    return 0;
}