/**
 * @file step3.h
 * @brief Header file containing function declarations and data structures for memory operations and second pass processing.
 *
 * This file includes function declarations and data structures for managing the memory table, performing the second pass of an assembler,
 * and writing memory data to binary and text files.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "step2.h"

/**
 * @struct memory
 * @brief Structure representing a memory word.
 *
 * This structure represents a memory word in an assembler, consisting of five 4-bit fields labeled A, B, C, D, and E.
 */
typedef struct
{
    unsigned int A : 4;  ///< 4-bit field A in the memory word.
    unsigned int B : 4;  ///< 4-bit field B in the memory word.
    unsigned int C : 4;  ///< 4-bit field C in the memory word.
    unsigned int D : 4;  ///< 4-bit field D in the memory word.
    unsigned int E : 4;  ///< 4-bit field E in the memory word.

    /* data */
} memory;

/**
 * @brief Prints the memory table to the console.
 *
 * This function outputs the current contents of the memory table to the console for debugging purposes.
 */
void printMemoryTable();

/**
 * @brief Performs the second pass of the assembler on the source file content.
 *
 * This function processes the source file content in the second pass of the assembler,
 * handling instructions and symbols that require further processing after the first pass.
 *
 * @param sourceFileContent The content of the source file to process in the second pass.
 */
void secondPass(char *sourceFileContent);

/**
 * @brief Writes the memory table to a binary file.
 *
 * This function writes the contents of the memory table to the specified binary file.
 *
 * @param filename The name of the file to write the memory data to in binary format.
 */
void writeMemoryTableToBinaryFile(const char *filename);

/**
 * @brief Writes the memory table to a text file.
 *
 * This function writes the contents of the memory table to the specified text file.
 *
 * @param filename The name of the file to write the memory data to in text format.
 */
void writeMemoryTableToTextFile(const char *filename);

#ifndef STEP3_H
#define STEP3_H

#endif // STEP3_H
