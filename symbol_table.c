#include "symbol_table.h"

Symbol symbolTable[SYMBOL_TABLE_SIZE];
int symbolTableCount = 0;
int IC = 0;
int DC = DATA_SEGMENT_START;

void addSymbol(char *label, int address, int isExternal, char *attributes) {
    strcpy(symbolTable[symbolTableCount].label, label);
    symbolTable[symbolTableCount].address = address;
    symbolTable[symbolTableCount].baseAddress = (address / 16) * 16; // Assuming base address is rounded to the nearest 16
    symbolTable[symbolTableCount].offset = address % 16;
    symbolTable[symbolTableCount].isExternal = isExternal;
    strcpy(symbolTable[symbolTableCount].attributes, attributes);
    symbolTableCount++;
}

int isLabel(char *line) {
    return line[strlen(line) - 1] == ':';
}

void processLine(char *line) {
    char *token = strtok(line, " \t\n");
    if (token == NULL) return;

    if (isLabel(token)) {
        token[strlen(token) - 1] = '\0'; // Remove the colon
        addSymbol(token, IC, 0, "code");
        token = strtok(NULL, " \t\n");
        if (token == NULL) return;
    }

    if (strcmp(token, ".data") == 0) {
        token = strtok(NULL, " \t\n");
        while (token != NULL) {
            DC += sizeof(int);
            token = strtok(NULL, " \t\n");
        }
    } else if (strcmp(token, ".string") == 0) {
        token = strtok(NULL, "\"\n");
        if (token != NULL) {
            DC += strlen(token) + 1;
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
        // Process as an instruction
        IC += INSTRUCTION_SIZE;
    }
}

void firstPass(FILE *sourceFile) {
    char line[MAX_LINE_LENGTH];
    while (fgets(line, MAX_LINE_LENGTH, sourceFile)) {
        processLine(line);
    }
}

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
// // }