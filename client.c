// the client side
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define IP "127.0.0.1"
#define PORT 5566
#define BUFFER_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in addr;
    char buffer[BUFFER_SIZE];
    char name[BUFFER_SIZE];

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

    while (1) {
        printf("Enter message: ");
        bzero(buffer, BUFFER_SIZE);
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = 0; // Remove trailing newline

        send(sock, buffer, strlen(buffer), 0);
        if (strcmp(buffer, "exit") == 0) {
             sleep(1);
            break;

        }



        bzero(buffer, BUFFER_SIZE);
        if (recv(sock, buffer, BUFFER_SIZE, 0) < 0) {
            perror("[-]Receive error");
            close(sock);
            exit(1);
        }
        printf("Server: %s\n", buffer);
    }

    close(sock);
    printf("Disconnected from the server.\n");

    return 0;
}