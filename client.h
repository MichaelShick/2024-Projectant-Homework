#ifndef CLIENT_H
#define CLIENT_H

#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

/**
 * @file client.h
 * @brief Header file for the client application.
 *
 * This file contains definitions and declarations for the client
 * application, including constants, and the function to receive messages.
 */

#define IP "127.0.0.1" ///< IP address of the server.
#define PORT 5566 ///< Port number for the server.
#define BUFFER_SIZE 1024 ///< Size of the buffer for messages.

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
void *receive_messages(void *sock_ptr);

#endif // CLIENT_H
