#include "frame_parser.h"

#include <netinet/in.h>

/**
 * @brief Parses a frame from a buffer and fills the frame header structure.
 *
 * This function takes a buffer containing the frame data and fills the provided
 * frame header structure with the parsed data.
 *
 * @param frame_header Pointer to the frame_header structure to be filled.
 * @param buffer Buffer containing the frame data.
 */
void parse_frame(struct frame_header *frame_header, unsigned char* buffer) {
    if (frame_header) {
        return;
    }

    memcpy(frame_header, buffer, sizeof(struct frame_header));
}

/**
 * @brief Prints the details of the frame header.
 *
 * This function prints the source IP, destination IP, and Ether type
 * from the provided frame header structure.
 *
 * @param frame_header Pointer to the frame_header structure containing the frame details.
 */
void print_frame(struct frame_header *frame_header) {
    printf("----------FRAME----------\n");
    printf("Source ip: %d.%d.%d.%d\n",
        frame_header->src_mac[0], frame_header->src_mac[1],
        frame_header->src_mac[2], frame_header->src_mac[3]);

    printf("Destination ip: %d.%d.%d.%d\n",
        frame_header->dest_mac[0], frame_header->dest_mac[1],
        frame_header->dest_mac[2], frame_header->dest_mac[3]);

    printf("Ether Type: ipv%d\n", (ntohs(frame_header->ether_type) == 0x0800)?4:6);
}
