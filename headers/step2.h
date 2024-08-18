/**
 * @file step2.h
 * @brief Header file containing function declarations and data structures for symbol table operations and instruction encoding.
 *
 * This file includes function declarations and data structures used for managing the symbol table,
 * processing assembly instructions, and handling the first pass of an assembler.
 *
 * Created by aviv on 15/08/2024.
 */

#ifndef STEP2_H
#define STEP2_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 100        ///< Maximum length of a line in the assembly source code.
#define SYMBOL_TABLE_SIZE 100      ///< Maximum size of the symbol table.
#define INSTRUCTION_SIZE 4         ///< Size of an instruction in bytes.
#define DATA_SEGMENT_START 100     ///< Starting address for the data segment.
#define MEMORY_SIZE 8192           ///< Size of the memory for the assembler.

typedef struct {
    char label[MAX_LINE_LENGTH];   ///< Label name.
    int address;                   ///< Address of the label.
    int baseAddress;               ///< Base address for relocatable symbols.
    int offset;                    ///< Offset for relocatable symbols.
    int isExternal;                ///< Flag indicating if the symbol is external (1) or not (0).
    char attributes[MAX_LINE_LENGTH]; ///< Attributes of the symbol.
} Symbol;

extern Symbol symbolTable[SYMBOL_TABLE_SIZE]; ///< The symbol table storing symbols.
extern int symbolTableCount;                  ///< Count of symbols in the symbol table.
extern int IC;                                ///< Instruction counter.
extern int DC;                                ///< Data counter.

/**
 * @brief Adds a symbol to the symbol table.
 *
 * This function adds a symbol with the specified label, address, external status, and attributes to the symbol table.
 *
 * @param label The label of the symbol.
 * @param address The address of the symbol.
 * @param isExternal Flag indicating if the symbol is external.
 * @param attributes The attributes associated with the symbol.
 */
void addSymbol(char *label, int address, int isExternal, char *attributes);

/**
 * @brief Checks if a line contains a label.
 *
 * This function determines if the given line of text contains a label.
 *
 * @param line The line of text to check.
 * @return 1 if the line contains a label, 0 otherwise.
 */
int isLabel(char *line);

/**
 * @brief Finds the address of a symbol in the symbol table.
 *
 * This function searches for the specified label in the symbol table and returns its address.
 *
 * @param label The label to search for.
 * @return The address of the symbol, or -1 if the symbol is not found.
 */
int findSymbolAddress(char *label);

/**
 * @brief Encodes an instruction into memory.
 *
 * This function encodes an instruction into the memory based on the provided parameters.
 *
 * @param address The address in memory where the instruction will be stored.
 * @param opcode The opcode of the instruction.
 * @param funct The function code (if applicable).
 * @param srcReg The source register number.
 * @param srcAddrMode The addressing mode of the source operand.
 * @param destReg The destination register number.
 * @param destAddrMode The addressing mode of the destination operand.
 */
void encodeInstruction(int address, int opcode, int funct, int srcReg, int srcAddrMode, int destReg, int destAddrMode);

/**
 * @brief Processes a single line of assembly code.
 *
 * This function processes a single line of assembly code, performing tasks such as parsing instructions and updating counters.
 *
 * @param line The line of assembly code to process.
 */
void processLine(char *line);

/**
 * @brief Performs the first pass of the assembler on the source file.
 *
 * This function performs the first pass on the provided source file, handling label definitions and updating the instruction and data counters.
 *
 * @param sourceFile The source file to process.
 */
void firstPass(FILE *sourceFile);

/**
 * @brief Performs the first pass of the assembler on a text string.
 *
 * This function performs the first pass on the provided text, handling label definitions and updating the instruction and data counters.
 *
 * @param text The text to process.
 */
void firstPass2(char *text);

/**
 * @brief Prints the symbol table to the console.
 *
 * This function outputs the contents of the symbol table to the console.
 */
void printSymbolTable();

/**
 * @brief Writes entry symbols to a text file.
 *
 * This function writes the entry symbols from the symbol table to the specified text file.
 *
 * @param filename The name of the file to write to.
 */
void writeEntrySymbolsToTextFile(const char *filename);

#endif // STEP2_H
