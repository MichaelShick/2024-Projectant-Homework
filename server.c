#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define IP "127.0.0.1"
#define PORT 5566
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 100

typedef struct {
    int sock;
    char name[BUFFER_SIZE];
} client_t;

client_t *clients[MAX_CLIENTS];
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void add_client(client_t *client) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (clients[i] == NULL) {
            clients[i] = client;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void remove_client(client_t *client) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (clients[i] == client) {
            clients[i] = NULL;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void broadcast_message(char *message, client_t *sender) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (clients[i] != NULL && clients[i] != sender) {
            send(clients[i]->sock, message, strlen(message), 0);
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void *handle_client(void *arg) {
    client_t *client = (client_t*)arg;
    char buffer[BUFFER_SIZE];
    int n;

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

    char welcome_message[BUFFER_SIZE];
    snprintf(welcome_message, BUFFER_SIZE, "[%s has joined the chat]\n", client->name);
    broadcast_message(welcome_message, client);

    while (1) {
        bzero(buffer, BUFFER_SIZE);
        if (recv(client->sock, buffer, BUFFER_SIZE, 0) <= 0) {
            perror("[-]Receive error");
            close(client->sock);
            remove_client(client);
            free(client);
            pthread_exit(NULL);
        }

        if (strcmp(buffer, "exit") == 0) {
            printf("[+]Client %s disconnected.\n\n", client->name);
            close(client->sock);
            remove_client(client);
            free(client);

            char exit_message[BUFFER_SIZE];
            snprintf(exit_message, BUFFER_SIZE, "[%s has left the chat]\n", client->name);
            broadcast_message(exit_message, client);
            pthread_exit(NULL);
        }

        char message[BUFFER_SIZE];
        snprintf(message, BUFFER_SIZE, "%s: %s\n", client->name, buffer);
        printf("%s", message);
        broadcast_message(message, client);
    }
}

int main() {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    pthread_t tid;

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("[-]Socket error");
        exit(1);
    }
    printf("[+]TCP server socket created.\n");

    memset(&server_addr, '\0', sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(IP);

    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("[-]Bind error");
        close(server_sock);
        exit(1);
    }
    printf("[+]Bind to the port number: %d\n", PORT);

    if (listen(server_sock, 5) < 0) {
        perror("[-]Listen error");
        close(server_sock);
        exit(1);
    }
    printf("Listening...\n");

    while (1) {
        addr_size = sizeof(client_addr);
        client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addr_size);
        if (client_sock < 0) {
            perror("[-]Accept error");
            continue;
        }

        client_t *client = (client_t *)malloc(sizeof(client_t));
        client->sock = client_sock;
        add_client(client);

        if (pthread_create(&tid, NULL, handle_client, (void*)client) != 0) {
            perror("[-]Thread creation error");
            close(client_sock);
            free(client);
        }
        pthread_detach(tid); // Detach the thread to handle cleanup automatically
    }

    close(server_sock);
    return 0;
}
