#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int client_socket;
    struct sockaddr_in server_address;

    // Create socket file descriptor
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) <= 0) {
        perror("inet_pton failed");
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("connect failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to the server.\n");

    char message[BUFFER_SIZE] = {0};

    while (1) {
        // Read input from the user
        printf("Enter message to send: ");
        fgets(message, BUFFER_SIZE, stdin);
        message[strcspn(message, "\n")] = 0;

        // Send message to the server
        if (strcmp(message, "SEND_FILE") == 0) {
            // Send SEND_FILE command to the server
            write(client_socket, message, strlen(message));

            // Read the file name from the user
            printf("Enter file name: ");
            fgets(message, BUFFER_SIZE, stdin);
            message[strcspn(message, "\n")] = 0;

            // Send the file name to the server
            write(client_socket, message, strlen(message));

            // Receive the response from the server
            memset(message, 0, BUFFER_SIZE);
            int num_bytes_received = read(client_socket, message, BUFFER_SIZE);
            if (num_bytes_received < 0) {
                perror("read failed");
                exit(EXIT_FAILURE);
            }

            if (strcmp(message, "FILE_NOT_FOUND") == 0) {
                printf("File not found on the server.\n");
            } else if (strcmp(message, "FILE_FOUND") == 0) {
                // Open the file for writing
                FILE* file = fopen("received_file", "wb");
                if (!file) {
                    perror("fopen failed");
                    exit(EXIT_FAILURE);
                }

                // Receive file data from the server
                while (1) {
                    memset(message, 0, BUFFER_SIZE);
                    int num_bytes_received = read(client_socket, message, BUFFER_SIZE);
                    if (num_bytes_received < 0) {
                        perror("read failed");
                        exit(EXIT_FAILURE);
                    }

                    if (strcmp(message, "FILE_SENT") == 0) {
                        printf("File received.\n");
                        break;
                    }

                    fwrite(message, 1, num_bytes_received, file);
                }

                fclose(file);
            }
        } else {
            // Send the message to the server
            write(client_socket, message, strlen(message));

            // Receive the response from the server
            memset(message, 0, BUFFER_SIZE);
            int num_bytes_received = read(client_socket, message, BUFFER_SIZE);
            if (num_bytes_received < 0) {
                perror("read failed");
                exit(EXIT_FAILURE);
            }

            printf("Server response: %s\n", message);
        }
    }

    // Close the socket
    close(client_socket);

    return 0;
}

