#ifndef IPV6_PARSER_H
#define IPV6_PARSER_H

#include <stdio.h>
#include <string.h>

#define FRAME_SIZE 14

/**
 * @brief Structure representing an IPv6 packet.
 *
 * This structure holds the various fields of an IPv6 packet, including
 * artt, payload length, next header, hop limit, source address, and destination address.
 */
struct ipv6_packet {
    unsigned char artt[4];       /**< Arbitrary four bytes */
    unsigned short payload_len;  /**< Payload length */
    unsigned char next_hdr;      /**< Next header */
    unsigned char hop_limit;     /**< Hop limit */
    unsigned char src_addr[16];  /**< Source address */
    unsigned char dest_addr[16]; /**< Destination address */
};

/**
 * @brief Parses an IPv6 packet from the given buffer.
 *
 * This function takes a buffer containing the frame data and fills the provided
 * ipv6_packet structure with the parsed IPv6 packet data.
 *
 * @param ipv6_packet Pointer to the ipv6_packet structure to be filled.
 * @param buffer Buffer containing the frame data.
 */
void parse_ipv6(struct ipv6_packet *ipv6_header, unsigned char *buffer);

/**
 * @brief Prints the details of the IPv6 packet.
 *
 * This function prints the various fields of the provided ipv6_packet structure
 * including artt, payload length, next header, hop limit, source address, and destination address.
 *
 * @param ipv6_packet Pointer to the ipv6_packet structure containing the IPv6 packet details.
 */
void print_ipv6(struct ipv6_packet *ipv6_header);

#endif // IPV6_PARSER_H
