#include "server.h"

/**
 * @brief Entry point for the server program.
 * 
 * This function sets up the server to listen for incoming client connections.
 * For each client that connects, it initializes the client information and
 * creates a new thread to handle communication with that client. The server
 * continues to accept new clients until an error occurs.
 * 
 * @return int Returns 0 on normal exit, or exits with an error code on failure.
 */
int main() {
    int server_sock, client_sock;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    pthread_t tid;

    // Create server socket
    server_sock = create_server_socket();

    // Accept clients
    while ((client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_addr_len)) != -1) {
        printf("Client connected: %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // Initialize client information
        ClientInfo *client_info = initialize_client_info(client_sock, &client_addr);
        if (client_info == NULL) {
            continue;
        }

        // Create a thread to handle the client
        if (pthread_create(&tid, NULL, handle_client, client_info) != 0) {
            perror("pthread_create");
            free(client_info);
            close(client_sock);
        } else {
            pthread_detach(tid); // Detach the thread so its resources are automatically released when it finishes
        }
    }

    if (client_sock == -1) {
        perror("accept");
        close(server_sock);
        exit(EXIT_FAILURE);
    }

    // Close the server socket
    close(server_sock);
    return 0;
}
