#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void handle_client(int client_socket);

int main() {
    int server_socket;
    struct sockaddr_in address;
    int opt = 1;

    // Create socket file descriptor
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind socket to address
    if (bind(server_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, 3) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Waiting for connection...\n");

    while (1) {
        int client_socket;
        struct sockaddr_in client_address;
        int client_address_len = sizeof(client_address);

        // Accept incoming connection
        if ((client_socket = accept(server_socket, (struct sockaddr *)&client_address, (socklen_t*)&client_address_len)) < 0) {
            perror("accept failed");
            exit(EXIT_FAILURE);
        }

        printf("Connected to client.\n");

        // Handle client
        handle_client(client_socket);
    }

    return 0;
}

void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE] = {0};
    int is_file_mode = 0;

    while (1) {
        // Receive message from client
        int num_bytes_received = read(client_socket, buffer, BUFFER_SIZE);
        if (num_bytes_received < 0) {
            perror("read failed");
            exit(EXIT_FAILURE);
        }

        // Check if client disconnected
        if (num_bytes_received == 0) {
            printf("Client disconnected.\n");
            break;
        }

        // Handle message
        if (strcmp(buffer, "SEND_FILE") == 0) {
            is_file_mode = 1;

            // Receive file name from client
            memset(buffer, 0, BUFFER_SIZE);
            int num_bytes_received = read(client_socket, buffer, BUFFER_SIZE);
            if (num_bytes_received < 0) {
                perror("read failed");
                exit(EXIT_FAILURE);
            }

            // Open file
            FILE* file = fopen(buffer, "rb");
            if (!file) {
                // File not found
                write(client_socket, "FILE_NOT_FOUND", strlen("FILE_NOT_FOUND"));
            } else {
                // Send file to client
                write(client_socket, "FILE_FOUND", strlen("FILE_FOUND"));

                char file_buffer[BUFFER_SIZE];
                int num_bytes_read;
                while ((num_bytes_read = fread(file_buffer, 1, BUFFER_SIZE, file)) > 0) {
                    write(client_socket, file_buffer, num_bytes_read);
                }

                fclose(file);

                // Send confirmation message to client
                write(client_socket, "FILE_SENT", strlen("FILE_SENT"));
            }
        } else if (strcmp(buffer, "EXIT") == 0) {
            // Client wants to exit
            printf("Client exited.\n");
            break;
        } else {
            printf("Client message: %s\n", buffer);

            // Send message
            char message[BUFFER_SIZE];
            printf("Enter message to send: ");
            fgets(message, BUFFER_SIZE, stdin);
            write(client_socket, message, strlen(message));
        }
    }

    close(client_socket);
}

