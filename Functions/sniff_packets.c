#include "sniff_packets.h"
#include "../Transport/tcp_parser.h"

/**
 * @brief Sniffs a specified number of packets and prints the details to the terminal.
 *
 * This function captures packets from a raw socket, parses them, and prints
 * the details of each packet to the terminal. It also counts the number of
 * loopback, TCP, and UDP packets.
 *
 * @param num The number of packets to sniff.
 */
void sniff_packets_to_terminal(int num) {
    unsigned char buffer[MAX_SIZE];

    int socket_id = create_socket();
    int loopback_counter = 0, tcp_counter = 0, udp_counter = 0;

    for(int i = 0; i < num; i++) {
        if (recvfrom(socket_id, buffer, sizeof(buffer), 0, NULL, NULL) == -1) {
            perror("recvfrom");
            exit(1);
        }

        // Checking for loopback
        if (buffer[0] == 0) {
            loopback_counter++;
        } else {
            // Packet header print
            printf("==========Packet %d==========\n\n", i + 1);
            struct frame_header *frame = (struct frame_header *)malloc(sizeof(struct frame_header));
            if (!frame) {
                exit(1);
            }

            parse_frame(frame, buffer);

            // Calling the print function
            print_frame(frame);

            switch (ntohs(frame->ether_type)) {
                case 0x0800: { // IPv4
                    struct ipv4_packet *ipv4 = (struct ipv4_packet *)malloc(sizeof(struct ipv4_packet));
                    if (!ipv4) {
                        free(frame); // Free frame before exiting
                        exit(1);
                    }
                    parse_ipv4(ipv4, buffer);

                    // Calling the print function
                    print_ipv4(ipv4);

                    // UDP/TCP
                    if (ipv4->protocol == 17) {
                        struct udp_hdr *udp = (struct udp_hdr *)malloc(sizeof(struct udp_hdr));
                        if (!udp) {
                            free(ipv4); // Free ipv4 before exiting
                            free(frame); // Free frame before exiting
                            exit(1);
                        }
                        parse_udp(udp, buffer, sizeof(ipv4));

                        // Calling the print function
                        print_udp(udp);
                        udp_counter++;

                        free(udp); // Free udp after use
                    } else {
                        struct tcp_parser *tcp = (struct tcp_parser *)malloc(sizeof(struct tcp_parser));
                        if (!tcp) {
                            free(ipv4); // Free ipv4 before exiting
                            free(frame); // Free frame before exiting
                            exit(1);
                        }

                        parse_tcp(tcp, buffer, sizeof(ipv4));

                        // Calling the print function
                        print_tcp(tcp);
                        tcp_counter++;

                        free(tcp); // Free tcp after use
                    }

                    free(ipv4); // Free ipv4 after use
                    break;
                }
                case 0x86DD: { // IPv6
                    struct ipv6_packet *ipv6 = (struct ipv6_packet *)malloc(sizeof(struct ipv6_packet));
                    if (!ipv6) {
                        free(frame); // Free frame before exiting
                        exit(1);
                    }

                    parse_ipv6(ipv6, buffer);

                    // Calling the print function
                    print_ipv6(ipv6);

                    // UDP/TTCP
                    if (ipv6->next_hdr == 17) {
                        struct udp_hdr *udp = (struct udp_hdr *)malloc(sizeof(struct udp_hdr));
                        if (!udp) {
                            free(ipv6); // Free ipv6 before exiting
                            free(frame); // Free frame before exiting
                            exit(1);
                        }
                        parse_udp(udp, buffer, sizeof(ipv6));

                        // Calling the print function
                        print_udp(udp);
                        udp_counter++;

                        free(udp); // Free udp after use
                    } else {
                        struct tcp_parser *tcp = (struct tcp_parser *)malloc(sizeof(struct tcp_parser));
                        if (!tcp) {
                            free(ipv6); // Free ipv6 before exiting
                            free(frame); // Free frame before exiting
                            exit(1);
                        }

                        parse_tcp(tcp, buffer, sizeof(ipv6));

                        // Calling the print function
                        print_tcp(tcp);
                        tcp_counter++;

                        free(tcp); // Free tcp after use
                    }

                    free(ipv6); // Free ipv6 after use
                    break;
                }
                default:
                    perror("Other ether type packet");
                    printf("\n\n");
            }

            free(frame); // Free frame after use
        }
    }
    printf("Amount of loopbacks caught: %d\n", loopback_counter);
    printf("TCP Counter: %d\n", tcp_counter);
    printf("UDP Counter: %d\n\n", udp_counter);

    close(socket_id); // Close the socket after use
}

/**
 * @brief Sniffs a specified number of packets and saves the details to a file.
 *
 * This function captures packets from a raw socket, parses them, and saves
 * the details of each packet to a file. Implementation is pending.
 *
 * @param num The number of packets to sniff.
 */
void sniff_packets_to_file(int num) {
    // Implementation pending
}
