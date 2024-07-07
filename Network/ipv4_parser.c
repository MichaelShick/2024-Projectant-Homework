#include <netinet/in.h>
#include "ipv4_parser.h"

/**
 * @brief Parses an IPv4 packet from the given buffer.
 *
 * This function takes a buffer containing the frame data and fills the provided
 * ipv4_packet structure with the parsed IPv4 packet data.
 *
 * @param ipv4 Pointer to the ipv4_packet structure to be filled.
 * @param buffer Buffer containing the frame data.
 */
void parse_ipv4(struct ipv4_packet *ipv4, unsigned char* buffer) {
    if (ipv4) {
        return;
    }

    memcpy(ipv4, buffer + FRAME_SIZE, sizeof(struct ipv4_packet));
}

/**
 * @brief Prints the details of the IPv4 packet.
 *
 * This function prints the various fields of the provided ipv4_packet structure
 * including version, header length, total length, identification, time to live,
 * protocol, header checksum, source IP address, and destination IP address.
 *
 * @param ipv4_packet Pointer to the ipv4_packet structure containing the IPv4 packet details.
 */
void print_ipv4(struct ipv4_packet *ipv4_packet) {
    printf("----------IPV4----------\n");
    printf("Version: %d\n", ipv4_packet->version);
    printf("Header Length: %d\n", ipv4_packet->header_len);
    printf("Total Length: %d\n", ipv4_packet->total_length);
    printf("Identification: %02x\n", ipv4_packet->identification);
    printf("Time to live: %d\n", ipv4_packet->time_to_live);
    printf("Protocol: %d\n", ipv4_packet->protocol);
    printf("Header check sum: %02x\n", ipv4_packet->header_checksum);
    printf("Source ip: %d.%d.%d.%d\n",
        ipv4_packet->source_ip_address[0], ipv4_packet->source_ip_address[1],
        ipv4_packet->source_ip_address[2], ipv4_packet->source_ip_address[3]);

    printf("Destination ip: %d.%d.%d.%d\n",
        ipv4_packet->dest_ip_address[0], ipv4_packet->dest_ip_address[1],
        ipv4_packet->dest_ip_address[2], ipv4_packet->dest_ip_address[3]);

    printf("\n");
}
