#ifndef CREATE_SOCKET_H
#define CREATE_SOCKET_H

#define MAX_SIZE 1518

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <linux/if_packet.h>
#include <net/ethernet.h> /* the L2 protocols */


/**
 * @brief Creates a raw socket.
 *
 * This function creates a raw socket using the AF_PACKET domain and SOCK_RAW type.
 * If the socket creation fails, an error message is printed, and the program exits.
 *
 * @return int The file descriptor for the created socket.
 */
int create_socket();

#endif // CREATE_SOCKET_H
