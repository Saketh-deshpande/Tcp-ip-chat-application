#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <unistd.h>

#include <arpa/inet.h>

int should_exit = 0;

void handle_client(int client_socket) {

    char buffer[1024];

    int bytes_received;

    while (!should_exit) {

        bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);

        if (bytes_received <= 0) {

            break;

        }

        buffer[bytes_received] = '\0';

        printf("Client: %s", buffer);

        if (strcmp(buffer, "exit\n") == 0) {

            printf("Client has exited the chat.\n");

            should_exit=1;

            break;

        }
        printf("You: ");

        fgets(buffer, sizeof(buffer), stdin);

        send(client_socket, buffer, strlen(buffer), 0);

    }
    close(client_socket);

}

int main() {

    int server_socket, client_socket;

    struct sockaddr_in server_addr, client_addr;

    socklen_t client_addr_len = sizeof(client_addr);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket == -1) {

        perror("Socket creation failed");

        exit(EXIT_FAILURE);

    }

    server_addr.sin_family = AF_INET;

    server_addr.sin_addr.s_addr = INADDR_ANY;

    server_addr.sin_port = htons(12345);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {

        perror("Bind failed");

        exit(EXIT_FAILURE);

    }

    if (listen(server_socket, 5) == -1) {

        perror("Listen failed");

        exit(EXIT_FAILURE);

    }
    printf("Server listening on port 12345\n");



    while (1) {

        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);

        if (client_socket == -1) {

            perror("Accept failed");

            continue;

        }
        printf("Accepted connection from %s:%d\n", inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));

        if (fork() == 0) {

            close(server_socket);

            handle_client(client_socket);

            exit(EXIT_SUCCESS);

        }

        close(client_socket);

         if (should_exit) {

            printf("Server is exiting.\n");

            break;

        }

    }

    close(server_socket);

    return 0;

}

