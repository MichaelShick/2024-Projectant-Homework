#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Provides a user interface for sniffing packets and displaying or saving the information.
 *
 * This function presents a menu to the user with options to print packet information,
 * save packet information to a file, do both, or exit the program. The user can
 * specify the number of packets to sniff for the print option.
 */
void user_interface();

#endif // USER_INTERFACE_H
