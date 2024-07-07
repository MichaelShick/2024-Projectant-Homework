#include "tcp_parser.h"

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
void parse_tcp(struct tcp_parser *tcp_parser, unsigned char *buffer, int ip_size) {
    if (tcp_parser) {// if tcp_parser is NULL
        return;
    }

    memcpy(tcp_parser, buffer + FRAME_SIZE + ip_size, sizeof(struct tcp_parser));
}

/**
 * @brief Prints the details of the TCP packet.
 *
 * This function prints the various fields of the provided tcp_parser structure
 * including source port, destination port, sequence number, acknowledgment number,
 * data offset, reserved and flags, window size, checksum, and urgent pointer.
 *
 * @param tcp_parser Pointer to the tcp_parser structure containing the TCP packet details.
 */
void print_tcp(struct tcp_parser *tcp_parser) {
    printf("----------TCP----------\n");
    printf("source port: %d\n", ntohs(tcp_parser->source_port));
    printf("destination port: %d\n", ntohs(tcp_parser->destination_port));
    printf("sequence number: %u\n", ntohl(tcp_parser->sequence_number));
    printf("acknowledgment number: %u\n", ntohl(tcp_parser->acknowledgment_number));
    printf("data offset and reserved and flags: %04x\n", ntohs(tcp_parser->data_offset_and_reserved_and_flags));
    printf("window size: %d\n", ntohs(tcp_parser->window_size));
    printf("checksum: %04x\n", ntohs(tcp_parser->checksum));
    printf("urgent pointer: %04x\n", ntohs(tcp_parser->urgent_pointer));
}
