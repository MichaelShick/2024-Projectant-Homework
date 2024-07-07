#ifndef FRAME_PARSER_H
#define FRAME_PARSER_H

#include <string.h>
#include <stdio.h>

#define MAX_FRAME_SIZE 1518

/**
 * @brief Structure representing the frame header.
 *
 * This structure holds the destination MAC address, source MAC address,
 * and Ether type for a network frame.
 */
struct frame_header {
    unsigned char dest_mac[6];   /**< Destination MAC address */
    unsigned char src_mac[6];    /**< Source MAC address */
    unsigned short ether_type;   /**< Ether type field */
};

/**
 * @brief Parses a frame from a buffer and fills the frame header structure.
 *
 * This function takes a buffer containing the frame data and fills the provided
 * frame header structure with the parsed data.
 *
 * @param frame_header Pointer to the frame_header structure to be filled.
 * @param buffer Buffer containing the frame data.
 */
void parse_frame(struct frame_header *frame_header, unsigned char* buffer);

/**
 * @brief Prints the details of the frame header.
 *
 * This function prints the source IP, destination IP, and Ether type
 * from the provided frame header structure.
 *
 * @param frame_header Pointer to the frame_header structure containing the frame details.
 */
void print_frame(struct frame_header *frame_header);

#endif // FRAME_PARSER_H
