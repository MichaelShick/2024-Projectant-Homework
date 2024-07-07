#include "user_interface.h"
#include "sniff_packets.h"

/**
 * @brief Provides a user interface for sniffing packets and displaying or saving the information.
 *
 * This function presents a menu to the user with options to print packet information,
 * save packet information to a file, do both, or exit the program. The user can
 * specify the number of packets to sniff for the print option.
 */
void user_interface() {
    bool loop = true;
    while(loop) {
        printf("Choose an option:\n=============\n"
               "1) Print information\n"
               "2) Save information to file\n"
               "3) Save and print information\n"
               "4) Exit\n\n");
        int pick, times;

        printf(">>");
        scanf("%d", &pick);

        while(getchar() != '\n'); // Clear the input buffer

        switch (pick) {
            case 1: // Call parse function - for print only
                printf("Enter the amount of packets you'd like to sniff: ");
                scanf("%d", &times);

                if (times > 0) {
                    sniff_packets_to_terminal(times);
                } else {
                    printf("Invalid number of packets.\n");
                }
                break;

            case 2: // Call parse function - for save to file
                printf("Enter the amount of packets you'd like to sniff: ");
                scanf("%d", &times);

                if (times > 0) {
                    sniff_packets_to_file(times);
                    printf("File Created\n");
                } else {
                    printf("Invalid number of packets.\n");
                }
                break;

            case 3: // Call parse function - for both printing and saving
                printf("Enter the amount of packets you'd like to sniff: ");
                scanf("%d", &times);

                if (times > 0) {
                    sniff_packets_to_terminal(times);
                    sniff_packets_to_file(times);
                    printf("File Created\n");
                } else {
                    printf("Invalid number of packets.\n");
                }
                break;

            case 4:
                puts("Program terminated.");
                loop = false;
                break;

            default:
                printf("Please choose a valid option\n\n");
        }
    }
}
