#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <arpa/inet.h>

/**
 * @file server.h
 * @brief Header file for the server-side implementation.
 */

/**
 * @brief The port number the server listens on.
 */
#define PORT 8383

/**
 * @brief The maximum number of clients that can connect simultaneously.
 */
#define MAX_CLIENTS 100

/**
 * @brief The maximum size of the buffer for client data.
 */
#define MAX_BUFFER_SIZE 1028

/**
 * @brief The path to the directory where log files are stored.
 */
#define SERVER_FOLDER_PATH "../log_files/"

/**
 * @brief The maximum size of the path buffer.
 */
#define PATH_SIZE 1024

/**
 * @brief The size of the temporary buffer used for receiving data.
 */
#define BUFF_SIZE 2

/**
 * @brief Structure to store client information.
 */
typedef struct {
    char ip[INET_ADDRSTRLEN]; /**< The IP address of the client. */
    char *buffer;             /**< Pointer to the buffer holding client's data. */
    size_t buffer_size;       /**< The current size of the data in the buffer. */
    size_t max_size;          /**< The maximum size allowed for the buffer. */
    int sock;                 /**< The socket file descriptor for the client. */
} ClientInfo;

/**
 * @brief Print the information of a disconnected client.
 * 
 * @param client_info Pointer to the structure containing client information.
 */
void print_client_info(const ClientInfo *client_info);

/**
 * @brief Write client information to a file.
 * 
 * @param client_info Pointer to the structure containing client information.
 * @param filename The name of the file to save the client information.
 */
void write_client_info_to_file(const ClientInfo *client_info, const char *filename);

/**
 * @brief Handle client communication in a separate thread.
 * 
 * @param client_info_ptr Pointer to the client information structure.
 * 
 * @return void* This function does not return a value.
 */
void *handle_client(void *client_info_ptr);

/**
 * @brief Create a server socket and bind it to a port.
 * 
 * @return int The file descriptor of the created server socket.
 */
int create_server_socket();

/**
 * @brief Initialize client information structure.
 * 
 * @param client_sock The socket file descriptor for the client.
 * @param client_addr Pointer to the sockaddr_in structure with the client's address.
 * 
 * @return ClientInfo* Pointer to the initialized ClientInfo structure.
 */
ClientInfo *initialize_client_info(int client_sock, struct sockaddr_in *client_addr);

#endif // SERVER_H
