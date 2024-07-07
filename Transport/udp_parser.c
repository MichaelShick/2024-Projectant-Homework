#include <stdio.h>
#include <netinet/in.h>
#include "udp_parser.h"

#define FRAME_SIZE 14

/**
 * @brief Parses a UDP packet from the given buffer.
 *
 * This function takes a buffer containing the frame data and fills the provided
 * udp_hdr structure with the parsed UDP packet data. The IP header size
 * is also taken into account to correctly locate the UDP header.
 *
 * @param udp Pointer to the udp_hdr structure to be filled.
 * @param buffer Buffer containing the frame data.
 * @param ip_size Size of the IP header.
 */
void parse_udp(struct udp_hdr *udp, unsigned char* buffer, int ip_size) {
    if (!udp) { // if udp is NULL
        return;
    }

    memcpy(udp, buffer + FRAME_SIZE + ip_size, sizeof(struct udp_hdr));
}

/**
 * @brief Prints the details of the UDP packet.
 *
 * This function prints the various fields of the provided udp_hdr structure
 * including source port, destination port, length, and checksum.
 *
 * @param udp_packet Pointer to the udp_hdr structure containing the UDP packet details.
 */
void print_udp(struct udp_hdr *udp_packet) {
    printf("----------UDP----------\n");
    printf("Source port: %d\n", ntohs(udp_packet->source_port));
    printf("Destination port: %d\n", ntohs(udp_packet->dest_port));
    printf("Length: %d\n", ntohs(udp_packet->length));
    printf("Checksum: %d\n\n", ntohs(udp_packet->checksum));
}
