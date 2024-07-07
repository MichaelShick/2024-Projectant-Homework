#ifndef SNIFF_PACKETS_H
#define SNIFF_PACKETS_H

#include "../Main/Headers.h"
#include "../Data Link/frame_parser.h"
#include "create_socket.h"
#include <stdio.h>

/**
 * @brief Sniffs a specified number of packets and prints the details to the terminal.
 *
 * This function captures packets from a raw socket, parses them, and prints
 * the details of each packet to the terminal. It also counts the number of
 * loopback, TCP, and UDP packets.
 *
 * @param num The number of packets to sniff.
 */
void sniff_packets_to_terminal(int num);

/**
 * @brief Sniffs a specified number of packets and saves the details to a file.
 *
 * This function captures packets from a raw socket, parses them, and saves
 * the details of each packet to a file. Implementation is pending.
 *
 * @param num The number of packets to sniff.
 */
void sniff_packets_to_file(int num);

#endif // SNIFF_PACKETS_H
