#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define IP "127.0.0.1"
#define PORT 5566
#define BUFFER_SIZE 1024

void *receive_messages(void *sock_ptr) {
    int sock = *(int*)sock_ptr;
    char buffer[BUFFER_SIZE];

    while (1) {
        bzero(buffer, BUFFER_SIZE);
        int bytes_received = recv(sock, buffer, BUFFER_SIZE, 0);
        if (bytes_received < 0) {
            perror("[-]Receive error");
            close(sock);
            exit(1);
        } else if (bytes_received == 0) {
            // Server disconnected
            printf("Server disconnected.\n");
            close(sock);
            exit(1);
        }

        // Clear the current input line and print the received message
        printf("\r\033[K"); // Clear the current line
        printf("%s", buffer);

        // Reprint the input prompt
        printf("Enter message: ");
        fflush(stdout); // Make sure "Enter message: " is printed immediately
    }
}

int main() {
    int sock;
    struct sockaddr_in addr;
    char buffer[BUFFER_SIZE];
    char name[BUFFER_SIZE];
    pthread_t tid;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("[-]Socket error");
        exit(1);
    }
    printf("[+]TCP client socket created.\n");

    memset(&addr, '\0', sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr(IP);

    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("[-]Connect error");
        close(sock);
        exit(1);
    }
    printf("Connected to the server.\n");

    printf("Enter your name: ");
    bzero(name, BUFFER_SIZE);
    fgets(name, BUFFER_SIZE, stdin);
    name[strcspn(name, "\n")] = 0; // Remove trailing newline

    send(sock, name, strlen(name), 0);

    if (pthread_create(&tid, NULL, receive_messages, &sock) != 0) {
        perror("[-]Thread creation error");
        close(sock);
        exit(1);
    }
    pthread_detach(tid); // Detach the thread to handle cleanup automatically

    while (1) {
        printf("Enter message: ");
        bzero(buffer, BUFFER_SIZE);
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = 0; // Remove trailing newline

        if (strcmp(buffer, "exit") == 0) {
            send(sock, buffer, strlen(buffer), 0);
            sleep(1); // Allow some time for the server to process the exit message
            break;
        }

        send(sock, buffer, strlen(buffer), 0);
    }

    close(sock);
    printf("Disconnected from the server.\n");

    return 0;
}