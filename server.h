/**
* @file server.h
 * @brief Header file for the server application.
 *
 * This file contains declarations and definitions for the server
 * application including client handling and communication functions.
 */

#ifndef SERVER_H
#define SERVER_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>

#define BUFFER_SIZE 1024 ///< Size of the buffer for messages.
#define MAX_CLIENTS 100 ///< Maximum number of clients supported.
#define IP "127.0.0.1" ///< IP address of the server.
#define PORT 5566 ///< Port number for the server.

/**
 * @struct client_t
 * @brief Structure to hold client information.
 */
typedef struct {
    int sock; ///< Socket descriptor for the client.
    char name[BUFFER_SIZE]; ///< Name of the client.
} client_t;

// Declare the clients array and mutex as external variables
extern client_t *clients_G[MAX_CLIENTS]; ///< Array to hold client information.
extern pthread_mutex_t clients_mutex_G; ///< Mutex for synchronizing access to client array.

/**
 * @brief Adds a client to the client list.
 *
 * @param client Pointer to the client to be added.
 */
void add_client(client_t *client);

/**
 * @brief Removes a client from the client list.
 *
 * @param client Pointer to the client to be removed.
 */
void remove_client(client_t *client);

/**
 * @brief Broadcasts a message to all clients except the sender.
 *
 * @param message The message to be broadcasted.
 * @param sender Pointer to the client sending the message.
 */
void broadcast_message(char *message, client_t *sender);

/**
 * @brief Handles communication with a client.
 *
 * @param arg Pointer to the client.
 * @return NULL
 */
void *handle_client(void *arg);

#endif // SERVER_H
