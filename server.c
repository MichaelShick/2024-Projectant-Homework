#include "server.h"

/**
 * @file server.c
 * @brief Source file for server functions.
 *
 * This file contains the implementations of the server functions
 * for managing clients and broadcasting messages.
 */

// Define the clients array and mutex in this source file
client_t *clients_G[MAX_CLIENTS] = {NULL};
pthread_mutex_t clients_mutex_G = PTHREAD_MUTEX_INITIALIZER;

/**
 * @brief Adds a client to the client list.
 *
 * This function locks the clients mutex, finds the first available
 * slot in the clients array, and adds the given client to that slot.
 *
 * @param client Pointer to the client to be added.
 */
void add_client(client_t *client) {
    pthread_mutex_lock(&clients_mutex_G);
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (clients_G[i] == NULL) {
            clients_G[i] = client;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex_G);
}

/**
 * @brief Removes a client from the client list.
 *
 * This function locks the clients mutex, finds the given client in
 * the clients array, and removes it by setting the slot to NULL.
 *
 * @param client Pointer to the client to be removed.
 */
void remove_client(client_t *client) {
    pthread_mutex_lock(&clients_mutex_G);
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (clients_G[i] == client) {
            clients_G[i] = NULL;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex_G);
}

/**
 * @brief Broadcasts a message to all clients except the sender.
 *
 * This function locks the clients mutex and sends the given message
 * to all clients in the clients array except for the sender.
 *
 * @param message The message to be broadcasted.
 * @param sender Pointer to the client sending the message.
 */
void broadcast_message(char *message, client_t *sender) {
    pthread_mutex_lock(&clients_mutex_G);
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (clients_G[i] != NULL && clients_G[i] != sender) {
            message[BUFFER_SIZE-1] = '\0';
            send(clients_G[i]->sock, message, BUFFER_SIZE, 0);
        }
    }
    pthread_mutex_unlock(&clients_mutex_G);
}

/**
 * @brief Handles communication with a client.
 *
 * This function runs in a separate thread for each client. It receives
 * messages from the client and broadcasts them to all other clients.
 * If the client sends "exit", it removes the client and closes the connection.
 *
 * @param arg Pointer to the client.
 * @return NULL
 */
void *handle_client(void *arg) {
    client_t *client = (client_t*)arg;
    char buffer[BUFFER_SIZE];
    char message[BUFFER_SIZE];
    char exit_message[BUFFER_SIZE];
    char name[BUFFER_SIZE];

    // Receive the client's name
    bzero(client->name, BUFFER_SIZE);
    if (recv(client->sock, client->name, BUFFER_SIZE, 0) <= 0) {
        perror("[-]Name receive error");
        close(client->sock);
        remove_client(client);
        free(client);
        pthread_exit(NULL);
    }
    printf("[+]Client connected with name: %s\n", client->name);
    strcpy(name, client->name);
    char welcome_message[BUFFER_SIZE];
    snprintf(welcome_message, BUFFER_SIZE, "[%s has joined the chat]\n", client->name);
    broadcast_message(welcome_message, client);

    while (true) {
        bzero(buffer, BUFFER_SIZE);
        if (recv(client->sock, buffer, BUFFER_SIZE, 0) <= 0) {
            perror("[-]Receive error");
            close(client->sock);
            remove_client(client);
            free(client);
            pthread_exit(NULL);
        }

        if (strcmp(buffer, "exit") == 0) {
            printf("[+]Client %s disconnected.\n", client->name);

            close(client->sock);
            remove_client(client);
            free(client);

            bzero(exit_message, BUFFER_SIZE);
            snprintf(exit_message, BUFFER_SIZE, "[%s has left the chat]\n", name);
            broadcast_message(exit_message, client);
            pthread_exit(NULL);
        }

        bzero(message, BUFFER_SIZE);
        snprintf(message, BUFFER_SIZE, "%s: %s\n", client->name, buffer);
        printf("%s", message);
        broadcast_message(message, client);
    }
}
