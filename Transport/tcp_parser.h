#ifndef TCP_PARSER_H
#define TCP_PARSER_H

#include <stdio.h>
#include <string.h>
#include <netinet/in.h>

#define FRAME_SIZE 14

/**
 * @brief Structure representing a TCP packet.
 *
 * This structure holds the various fields of a TCP packet, including
 * source port, destination port, sequence number, acknowledgment number,
 * data offset, reserved and flags, window size, checksum, and urgent pointer.
 */
struct tcp_parser {
    unsigned short source_port;   /**< Source port */
    unsigned short destination_port; /**< Destination port */
    int sequence_number;          /**< Sequence number */
    int acknowledgment_number;    /**< Acknowledgment number */
    unsigned short data_offset_and_reserved_and_flags; /**< Data offset, reserved, and flags */
    unsigned short window_size;   /**< Window size */
    unsigned short checksum;      /**< Checksum */
    unsigned short urgent_pointer; /**< Urgent pointer */
};

/**
 * @brief Parses a TCP packet from the given buffer.
 *
 * This function takes a buffer containing the frame data and fills the provided
 * tcp_parser structure with the parsed TCP packet data. The IP header size
 * is also taken into account to correctly locate the TCP header.
 *
 * @param tcp_parser Pointer to the tcp_parser structure to be filled.
 * @param buffer Buffer containing the frame data.
 * @param ip_size Size of the IP header.
 */
void parse_tcp(struct tcp_parser *tcp_parser, unsigned char *buffer, int ip_size);

/**
 * @brief Prints the details of the TCP packet.
 *
 * This function prints the various fields of the provided tcp_parser structure
 * including source port, destination port, sequence number, acknowledgment number,
 * data offset, reserved and flags, window size, checksum, and urgent pointer.
 *
 * @param tcp_parser Pointer to the tcp_parser structure containing the TCP packet details.
 */
void print_tcp(struct tcp_parser *tcp_parser);

#endif // TCP_PARSER_H
