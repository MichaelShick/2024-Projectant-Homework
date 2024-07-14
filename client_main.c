#include "client.h"

/**
 * @file client.c
 * @brief Main entry point for the client application.
 *
 * This file contains the main function which connects to the server,
 * sends the client's name, and handles user input for sending messages.
 */

/**
 * @brief Main function to start the client.
 *
 * This function initializes the client socket, connects to the server,
 * sends the client's name, and creates a thread to handle incoming messages.
 * The client can send messages to the server and exit by typing "exit".
 *
 * @return int Returns 0 on successful execution.
 */
int main() {
    int sock;
    struct sockaddr_in addr;
    char buffer[BUFFER_SIZE];
    char name[BUFFER_SIZE];
    pthread_t tid;

    // Create the client socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("[-]Socket error");
        exit(EXIT_FAILURE);
    }
    printf("[+]TCP client socket created.\n");

    // Initialize the server address structure
    memset(&addr, '\0', sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr(IP);

    // Connect to the server
    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("[-]Connect error");
        close(sock);
        exit(EXIT_FAILURE);
    }
    printf("Connected to the server.\n");

    // Get the client's name and send it to the server
    printf("Enter your name: ");
    bzero(name, BUFFER_SIZE);
    fgets(name, BUFFER_SIZE, stdin);
    name[strcspn(name, "\n")] = 0; // Remove trailing newline

    send(sock, name, strlen(name), 0);

    // Create a thread to receive messages from the server
    if (pthread_create(&tid, NULL, receive_messages, &sock) != 0) {
        perror("[-]Thread creation error");
        close(sock);
        exit(EXIT_FAILURE);
    }
    pthread_detach(tid); // Detach the thread to handle cleanup automatically

    // Loop to send messages to the server
    while (true) {
        printf("You: ");
        bzero(buffer, BUFFER_SIZE);
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = 0; // Remove trailing newline

        if (strcmp(buffer, "exit") == 0) {
            sleep(1); // Allow some time for the server to process the exit message
            send(sock, buffer, strlen(buffer), 0);
            break;
        }

        send(sock, buffer, strlen(buffer), 0);
    }

    // Close the socket and exit
    close(sock);
    printf("Disconnected from the server.\n");

    return 0;
}
