#ifndef IPV4_PARSER_H
#define IPV4_PARSER_H

#include <stdio.h>
#include <string.h>

#define FRAME_SIZE 14

/**
 * @brief Structure representing an IPv4 packet.
 *
 * This structure holds the various fields of an IPv4 packet, including
 * version, header length, type of service, total length, identification,
 * flags and fragment offset, time to live, protocol, header checksum,
 * source IP address, and destination IP address.
 */
struct ipv4_packet {
    unsigned char version:4, header_len:4; /**< Version and header length */
    unsigned char type_of_service;          /**< Type of service */
    unsigned short total_length;            /**< Total length */
    unsigned short identification;          /**< Identification */
    unsigned short flags_and_fragment_offset; /**< Flags and fragment offset */
    unsigned char time_to_live;             /**< Time to live */
    unsigned char protocol;                 /**< Protocol */
    unsigned short header_checksum;         /**< Header checksum */
    unsigned char source_ip_address[4];     /**< Source IP address */
    unsigned char dest_ip_address[4];       /**< Destination IP address */
};

/**
 * @brief Parses an IPv4 packet from the given buffer.
 *
 * This function takes a buffer containing the frame data and fills the provided
 * ipv4_packet structure with the parsed IPv4 packet data.
 *
 * @param ipv4 Pointer to the ipv4_packet structure to be filled.
 * @param buffer Buffer containing the frame data.
 */
void parse_ipv4(struct ipv4_packet *ipv4, unsigned char* buffer);

/**
 * @brief Prints the details of the IPv4 packet.
 *
 * This function prints the various fields of the provided ipv4_packet structure
 * including version, header length, total length, identification, time to live,
 * protocol, header checksum, source IP address, and destination IP address.
 *
 * @param ipv4_packet Pointer to the ipv4_packet structure containing the IPv4 packet details.
 */
void print_ipv4(struct ipv4_packet *ipv4_packet);

#endif // IPV4_PARSER_H
