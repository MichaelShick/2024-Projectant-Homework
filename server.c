
// the server side
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define IP "127.0.0.1"
#define PORT 5566
#define BUFFER_SIZE 1024

void *handle_client(void *client_socket) {
    int client_sock = *(int*)client_socket;
    char buffer[BUFFER_SIZE];
    char client_name[BUFFER_SIZE];
    int n;

    // Receive the client's name
    bzero(client_name, BUFFER_SIZE);
    if (recv(client_sock, client_name, BUFFER_SIZE, 0) <= 0) {
        perror("[-]Name receive error");
        close(client_sock);
        pthread_exit(NULL);
    }
    printf("[+]Client connected with name: %s\n", client_name);

    while (1) {

        bzero(buffer, BUFFER_SIZE);
        if (recv(client_sock, buffer, BUFFER_SIZE, 0) <= 0) {
            perror("[-]Receive error");
            close(client_sock);
            pthread_exit(NULL);
        }

        if (strcmp(buffer, "exit") == 0) {
            printf("[+]Client %s disconnected.\n\n", client_name);
            close(client_sock);
            pthread_exit(NULL);
        }
        else {
            printf("%s: %s\n", client_name, buffer);
        }
        if (strcmp(buffer, "exit") == 0) {
            printf("[+]Client %s disconnected.\n\n", client_name);
            close(client_sock);
            pthread_exit(NULL);
        }

        bzero(buffer, BUFFER_SIZE);
        strcpy(buffer, "Message received.");
        send(client_sock, buffer, strlen(buffer), 0);
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

        if (pthread_create(&tid, NULL, handle_client, &client_sock) != 0) {
            perror("[-]Thread creation error");
            close(client_sock);
        }
        pthread_detach(tid); // Detach the thread to handle cleanup automatically
    }

    close(server_sock);
    return 0;
}