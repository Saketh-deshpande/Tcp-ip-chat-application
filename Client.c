#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <unistd.h>

#include <arpa/inet.h>

int main() {

    int client_socket;

    struct sockaddr_in server_addr;

    char buffer[1024];

    int bytes_received;

    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (client_socket == -1) {

        perror("Socket creation failed");

        exit(EXIT_FAILURE);

    }

    server_addr.sin_family = AF_INET;

    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    server_addr.sin_port = htons(12345);

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {

        perror("Connect failed");

        exit(EXIT_FAILURE);

    }

	printf("Welcome to the chat. Type 'exit' to quit.\n");

    while (1) {

        printf("You: ");

        fgets(buffer, sizeof(buffer), stdin);

        if (strcmp(buffer, "exit\n")==0) {

           send(client_socket, buffer, strlen(buffer), 0);

	   break;

        }

        send(client_socket, buffer, strlen(buffer), 0);

        bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);

        if (bytes_received <= 0) {

            break;

        }

        buffer[bytes_received] = '\0';

        printf("Server: %s", buffer);

        }

    close(client_socket);

    return 0;

}

