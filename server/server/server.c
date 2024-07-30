#include "server.h"

/**
 * @brief Print the information of a disconnected client.
 * 
 * This function displays the IP address, socket number, buffer size, and buffer content
 * of a client that has disconnected from the server.
 * 
 * @param client_info Pointer to the structure containing client information.
 */
void print_client_info(const ClientInfo *client_info) {
    printf("Client disconnected: %s:%d\n", client_info->ip, client_info->sock);
    printf("Buffer size: %zu\n", client_info->buffer_size);
    if (client_info->buffer_size > 0) {
        printf("Buffer content: %s\n", client_info->buffer);
    }
}

/**
 * @brief Write client information to a file.
 * 
 * This function saves the details of a disconnected client, including their IP address,
 * socket number, buffer size, and buffer content, to a specified file.
 * 
 * @param client_info Pointer to the structure containing client information.
 * @param filename The name of the file to save the client information.
 */
void write_client_info_to_file(const ClientInfo *client_info, const char *filename) {
    char path[PATH_SIZE];
    snprintf(path, sizeof(path), "%s%s", SERVER_FOLDER_PATH, filename);
    printf("Saving in %s\n", path);
    FILE *file = fopen(path, "a");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    fprintf(file, "Client disconnected: %s:%d\n", client_info->ip, client_info->sock);
    fprintf(file, "Buffer size: %zu\n", client_info->buffer_size);
    if (client_info->buffer_size > 0) {
        fprintf(file, "Buffer content: %s\n", client_info->buffer);
    }
    fclose(file);
}

/**
 * @brief Handle client communication in a separate thread.
 * 
 * This function handles the reception of data from a connected client. It stores the data
 * in a buffer and offers options to either print the data to the console or save it to a file
 * when the buffer exceeds a certain size or when the client disconnects.
 * 
 * @param client_info_ptr Pointer to the client information structure.
 * 
 * @return void* This function does not return a value.
 */
void *handle_client(void *client_info_ptr) {
    ClientInfo *client_info = (ClientInfo *)client_info_ptr;
    int sock = client_info->sock;
    char temp_buffer[BUFF_SIZE];
    int bytes_read;
    int write_to_file; // 1 for writing to file, 0 for printing to screen

    while ((bytes_read = recv(sock, temp_buffer, sizeof(temp_buffer), 0)) > 0) {
        temp_buffer[bytes_read] = '\0';

        // Reallocate buffer to hold new data
        client_info->buffer = realloc(client_info->buffer, client_info->buffer_size + bytes_read + 1);
        if (client_info->buffer == NULL) {
            perror("realloc");
            close(sock);
            free(client_info);
            pthread_exit(NULL);
        }

        // Append new data to buffer
        memcpy(client_info->buffer + client_info->buffer_size, temp_buffer, bytes_read + 1);
        client_info->buffer_size += bytes_read;

        printf("Received key from %s:%d: %s\n", client_info->ip, client_info->sock, temp_buffer);

        // Check if buffer size exceeds max_size
        if (client_info->buffer_size > client_info->max_size) {
            printf("Buffer size exceeded %zu bytes.\n", client_info->max_size);
            printf("Would you like to print or save the data to a text file? (0-print, 1-file): ");
            scanf("%d", &write_to_file);

            if (write_to_file) {
                write_client_info_to_file(client_info, "client_log.txt");
            } else {
                print_client_info(client_info);
            }

            // Reset buffer
            free(client_info->buffer);
            client_info->buffer = NULL;
            client_info->buffer_size = 0;
        }
    }

    if (bytes_read == -1) {
        perror("recv");
    }

    // Print or write client info before closing
    printf("Would you like to print or save the data to a text file? (0-print, 1-file): ");
    scanf("%d", &write_to_file);

    if (write_to_file) {
        write_client_info_to_file(client_info, "client_log.txt");
    } else {
        print_client_info(client_info);
    }

    close(sock);
    free(client_info->buffer);
    free(client_info);
    pthread_exit(NULL);
}

/**
 * @brief Create a server socket and bind it to a port.
 * 
 * This function creates a socket for the server and binds it to a specified port.
 * It also sets up the socket to listen for incoming client connections.
 * 
 * @return int The file descriptor of the created server socket.
 */
int create_server_socket() {
    int server_sock;
    struct sockaddr_in server_addr;

    // Create socket
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Bind
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        close(server_sock);
        exit(EXIT_FAILURE);
    }

    // Listen
    if (listen(server_sock, MAX_CLIENTS) == -1) {
        perror("listen");
        close(server_sock);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    return server_sock;
}

/**
 * @brief Initialize client information structure.
 * 
 * This function allocates memory for a ClientInfo structure and initializes it with
 * the client's socket and address information. It also sets the initial buffer size.
 * 
 * @param client_sock The socket file descriptor for the client.
 * @param client_addr Pointer to the sockaddr_in structure with the client's address.
 * 
 * @return ClientInfo* Pointer to the initialized ClientInfo structure.
 */
ClientInfo *initialize_client_info(int client_sock, struct sockaddr_in *client_addr) {
    ClientInfo *client_info = malloc(sizeof(ClientInfo));
    if (client_info == NULL) {
        perror("malloc");
        close(client_sock);
        return NULL;
    }

    inet_ntop(AF_INET, &client_addr->sin_addr, client_info->ip, INET_ADDRSTRLEN);
    client_info->buffer = NULL;
    client_info->buffer_size = 0;
    client_info->max_size = MAX_BUFFER_SIZE;
    client_info->sock = client_sock;

    return client_info;
}
