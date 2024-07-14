#include "client.h"

/**
 * @file client_receive.c
 * @brief Implementation of the message receiving function for the client.
 *
 * This file contains the function that handles receiving messages from the server
 * and displaying them to the user.
 */

/**
 * @brief Function to receive messages from the server.
 *
 * This function runs in a separate thread, continuously receiving messages from the
 * server and printing them to the standard output. If the server disconnects, it
 * closes the socket and exits.
 *
 * @param sock_ptr Pointer to the client's socket.
 * @return void* Always returns NULL.
 */
void *receive_messages(void *sock_ptr) {
    int sock = *(int*)sock_ptr;
    char buffer[BUFFER_SIZE];

    while (true) {
        bzero(buffer, BUFFER_SIZE);
        int bytes_received = recv(sock, buffer, BUFFER_SIZE, 0);
        if (bytes_received < 0) {
            perror("[-]Receive error");
            close(sock);
            exit(EXIT_FAILURE);
        } else if (bytes_received == 0) {
            // Server disconnected
            printf("Server disconnected.\n");
            close(sock);
            exit(EXIT_FAILURE);
        }

        // Clear the current input line and print the received message
        printf("\r\033[K"); // Clear the current line
        printf("%s", buffer);

        // Reprint the input prompt
        printf("You: ");
        fflush(stdout); // Make sure "You: " is printed immediately
    }
}
