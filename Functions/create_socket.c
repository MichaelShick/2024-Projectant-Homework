#include "create_socket.h"

/**
 * @brief Creates a raw socket.
 *
 * This function creates a raw socket using the AF_PACKET domain and SOCK_RAW type.
 * If the socket creation fails, an error message is printed, and the program exits.
 *
 * @return int The file descriptor for the created socket.
 */
int create_socket() {
    int raw;

    // Create a socket
    if ((raw = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) == -1) {
        perror("Socket failed");
        exit(1);
    }
    return raw;
}
