#include "./ipv6_parser.h"

/**
 * @brief Parses an IPv6 packet from the given buffer.
 *
 * This function takes a buffer containing the frame data and fills the provided
 * ipv6_packet structure with the parsed IPv6 packet data.
 *
 * @param ipv6_packet Pointer to the ipv6_packet structure to be filled.
 * @param buffer Buffer containing the frame data.
 */
void parse_ipv6(struct ipv6_packet *ipv6_packet, unsigned char *buffer) {
    if (ipv6_packet) {
        return;
    }

    memcpy(ipv6_packet, buffer + FRAME_SIZE, sizeof(struct ipv6_packet));
}

/**
 * @brief Prints the details of the IPv6 packet.
 *
 * This function prints the various fields of the provided ipv6_packet structure
 * including artt, next header, hop limit, source address, and destination address.
 *
 * @param ipv6_packet Pointer to the ipv6_packet structure containing the IPv6 packet details.
 */
void print_ipv6(struct ipv6_packet *ipv6_packet) {
    printf("----------IPV6----------\n");
    printf("artts: %02x%02x%02x%02x\n",
                        ipv6_packet->artt[0], ipv6_packet->artt[1],
                        ipv6_packet->artt[2], ipv6_packet->artt[3]);

    printf("next header: %02x\n", ipv6_packet->next_hdr);
    printf("hop limit: %02x\n", ipv6_packet->hop_limit);
    printf("source address: ");

    for (int i = 0; i < 16; i++) {
        printf("%02x", ipv6_packet->src_addr[i]);
        if (i % 2 && i != 15) printf(":");
    }

    printf("\n");
    printf("destination address: ");

    for (int i = 0; i < 16; i++) {
        printf("%02x", ipv6_packet->dest_addr[i]);
        if (i % 2 && i != 15) printf(":");
    }
    printf("\n");
}
