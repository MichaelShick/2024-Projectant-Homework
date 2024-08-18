#include "../headers/step2.h"

/**
 * @brief Global symbol table to store labels and their corresponding addresses and attributes.
 */
Symbol symbolTable[SYMBOL_TABLE_SIZE];

/**
 * @brief Global counter for the number of symbols in the symbol table.
 */
int symbolTableCount = 0;

/**
 * @brief Instruction counter, initialized to the start of the data segment.
 */
int IC = DATA_SEGMENT_START;

/**
 * @brief Data counter, initialized to the start of the data segment.
 */
int DC = DATA_SEGMENT_START;

/**
 * @brief Adds a symbol to the symbol table.
 *
 * This function adds a new symbol to the symbol table with the specified label, address, external status, and attributes.
 *
 * @param label The label of the symbol.
 * @param address The address of the symbol.
 * @param isExternal Indicates if the symbol is external (1) or not (0).
 * @param attributes The attributes associated with the symbol (e.g., "data", "code").
 */
void addSymbol(char *label, int address, int isExternal, char *attributes) {
    strcpy(symbolTable[symbolTableCount].label, label);
    symbolTable[symbolTableCount].address = address;
    symbolTable[symbolTableCount].baseAddress = (address / 16) * 16; // Assuming base address is rounded to the nearest 16
    symbolTable[symbolTableCount].offset = address % 16;
    symbolTable[symbolTableCount].isExternal = isExternal;
    strcpy(symbolTable[symbolTableCount].attributes, attributes);
    symbolTableCount++;
}

/**
 * @brief Checks if a line of code contains a label.
 *
 * This function checks if the given line ends with a colon, indicating that it is a label.
 *
 * @param line The line of code to check.
 * @return 1 if the line is a label, 0 otherwise.
 */
int isLabel(char *line) {
    return line[strlen(line) - 1] == ':';
}

/**
 * @brief Finds the address of a symbol in the symbol table.
 *
 * This function searches the symbol table for the given label and returns its address if found.
 * The function skips symbols marked as "entry".
 *
 * @param label The label to search for.
 * @return The address of the symbol if found, or -1 if the symbol is not found.
 */
int findSymbolAddress(char *label) {
    for (int i = 0; i < symbolTableCount; i++) {
        if (strcmp(symbolTable[i].label, label) == 0 && strcmp(symbolTable[i].attributes, "entry") != 0) {
            return symbolTable[i].address;
        }
    }
    return -1; // If not found
}

/**
 * @brief Encodes an instruction into a memory word.
 *
 * This function encodes the provided opcode, function code, source register, source addressing mode,
 * destination register, and destination addressing mode into a 24-bit instruction.
 *
 * @param address The memory address where the instruction will be stored.
 * @param opcode The opcode of the instruction.
 * @param funct The function code of the instruction.
 * @param srcReg The source register.
 * @param srcAddrMode The source addressing mode.
 * @param destReg The destination register.
 * @param destAddrMode The destination addressing mode.
 */
void encodeInstruction(int address, int opcode, int funct, int srcReg, int srcAddrMode, int destReg, int destAddrMode) {
    unsigned int instruction = 0;
    instruction |= (opcode << 16);          // Set opcode
    instruction |= (funct << 12);           // Set funct
    instruction |= (srcReg << 8);           // Set source register
    instruction |= (srcAddrMode << 6);      // Set source addressing mode
    instruction |= (destReg << 2);          // Set destination register
    instruction |= destAddrMode;            // Set destination addressing mode
   // memorytable[address] = instruction;
}

/**
 * @brief Processes a single line of assembly code.
 *
 * This function processes a single line of assembly code, handling labels, directives (e.g., .data, .string, .entry, .extern),
 * and instructions. It updates the instruction counter (IC) and data counter (DC) accordingly.
 *
 * @param line The line of assembly code to process.
 */
void processLine(char *line) {
    char *token = strtok(line, " \t\n");
    char *label = NULL;
    if (token == NULL) return;

    if (isLabel(token)) {
        token[strlen(token) - 1] = '\0'; // Remove the colon
        label = token; // Store the label for later use
        token = strtok(NULL, " \t\n");
        if (token == NULL) return;
    }

    if (strcmp(token, ".data") == 0) {
        if (label != NULL) {
            addSymbol(label, DC, 0, "data");
        }
        token = strtok(NULL, " \t\n");
        while (token != NULL) {
            DC += sizeof(int);
            token = strtok(NULL, " \t\n");
        }
    } else if (strcmp(token, ".string") == 0) {
        if (label != NULL) {
            addSymbol(label, DC, 0, "data");
        }
        token = strtok(NULL, "\"\n");
        if (token != NULL) {
            DC += strlen(token) + 1; // +1 for the null terminator
        }
    } else if (strcmp(token, ".entry") == 0) {
        token = strtok(NULL, " \t\n");
        if (token != NULL) {
            addSymbol(token, 0, 0, "entry");
        }
    } else if (strcmp(token, ".extern") == 0) {
        token = strtok(NULL, " \t\n");
        if (token != NULL) {
            addSymbol(token, 0, 1, "external");
        }
    } else {
        if (label != NULL) {
            addSymbol(label, IC, 0, "code");
        }
        // Process as an instruction
        IC += INSTRUCTION_SIZE;
    }
}

/**
 * @brief Performs the first pass of the assembler on a source file.
 *
 * This function reads each line of the provided source file, processing labels, directives, and instructions
 * to update the symbol table, instruction counter (IC), and data counter (DC).
 *
 * @param sourceFile The source file to process.
 */
void firstPass(FILE *sourceFile) {
    char line[MAX_LINE_LENGTH];
    while (fgets(line, MAX_LINE_LENGTH, sourceFile)) {
        processLine(line);
    }
}

/**
 * @brief Performs the first pass of the assembler on a string.
 *
 * This function reads each line from the provided string, processing labels, directives, and instructions
 * to update the symbol table, instruction counter (IC), and data counter (DC).
 *
 * @param text The input string containing the assembly code.
 */
void firstPass2(char *text) {
    char line[MAX_LINE_LENGTH];
    char *token;
    char *rest = text;
    while ((token = strtok_r(rest, "\n", &rest))) {
        strncpy(line, token, MAX_LINE_LENGTH - 1);
        line[MAX_LINE_LENGTH - 1] = '\0';  // Ensure the line is null-terminated
        processLine(line);
    }
}

/**
 * @brief Prints the symbol table to the console.
 *
 * This function outputs the contents of the symbol table, including the label, address, base address, offset,
 * external flag, and attributes for each symbol.
 */
void printSymbolTable() {
    printf("Symbol Table:\n");
    printf("%-10s %-10s %-10s %-10s %-10s %-20s\n", "Symbol", "Value", "Base", "Offset", "External", "Attributes");
    for (int i = 0; i < symbolTableCount; i++) {
        printf("%-10s %-10d %-10d %-10d %-10d %-20s\n",
               symbolTable[i].label,
               symbolTable[i].address,
               symbolTable[i].baseAddress,
               symbolTable[i].offset,
               symbolTable[i].isExternal,
               symbolTable[i].attributes);
    }
}

/**
 * @brief Writes entry symbols to a text file.
 *
 * This function writes the entry symbols from the symbol table to the specified text file.
 * Each entry symbol is written along with its base address and offset.
 *
 * @param filename The name of the file to write the entry symbols to.
 */
void writeEntrySymbolsToTextFile(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    for (int i = 0; i < symbolTableCount; i++) {
        if (strcmp(symbolTable[i].attributes, "entry") == 0) {
            for (int j = 0; j < symbolTableCount; j++) {
                if ((strcmp(symbolTable[j].label, symbolTable[i].label) == 0 ) && symbolTable[j].address != 0){
                    fprintf(file, "%s ,%d,%d\n", symbolTable[j].label, symbolTable[j].baseAddress, symbolTable[j].offset);
                    break;
                }
            }
        }
    }

    fclose(file);
}
