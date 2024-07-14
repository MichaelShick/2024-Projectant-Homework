#include "server.h"

/**
 * @file main.c
 * @brief Main entry point for the server application.
 *
 * This file contains the main function which initializes the server,
 * accepts client connections, and creates threads to handle each client.
 */

/**
 * @brief Main function to start the server.
 *
 * This function initializes the server socket, binds to the specified port,
 * listens for incoming connections, and creates a new thread to handle each
 * connected client.
 *
 * @return int Returns 0 on successful execution, 1 on error.
 */
int main() {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    pthread_t tid;

    // Initialize the clients mutex
    if (pthread_mutex_init(&clients_mutex_G, NULL) != 0) {
        perror("Mutex creation has failed\n");
        return 1;
    }

    // Create the server socket
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("[-]Socket error");
        exit(EXIT_FAILURE);
    }
    printf("[+]TCP server socket created.\n");

    // Initialize the server address structure
    memset(&server_addr, '\0', sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(IP);

    // Bind the socket to the specified port
    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("[-]Bind error");
        close(server_sock);
        exit(EXIT_FAILURE);
    }
    printf("[+]Bind to the port number: %d\n", PORT);

    // Listen for incoming connections
    if (listen(server_sock, 5) < 0) {
        perror("[-]Listen error");
        close(server_sock);
        exit(EXIT_FAILURE);
    }
    printf("Listening...\n");

    // Accept and handle incoming client connections
    while (true) {
        addr_size = sizeof(client_addr);
        client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addr_size);
        if (client_sock < 0) {
            perror("[-]Accept error");
            continue;
        }

        // Allocate memory for the new client and add it to the client list
        client_t *client = (client_t *)malloc(sizeof(client_t));
        client->sock = client_sock;
        add_client(client);

        // Create a new thread to handle the client
        if (pthread_create(&tid, NULL, handle_client, (void*)client) != 0) {
            perror("[-]Thread creation error");
            close(client_sock);
            free(client);
        }
        pthread_detach(tid); // Detach the thread to handle cleanup automatically
    }

    // Close the server socket
    close(server_sock);
    return 0;
}
