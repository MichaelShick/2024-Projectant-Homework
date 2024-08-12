#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 100
#define SYMBOL_TABLE_SIZE 100
#define INSTRUCTION_SIZE 4
#define DATA_SEGMENT_START 100

typedef struct {
    char label[MAX_LINE_LENGTH];
    int address;
    int baseAddress;
    int offset;
    int isExternal;
    char attributes[MAX_LINE_LENGTH];
} Symbol;

extern Symbol symbolTable[SYMBOL_TABLE_SIZE];
extern int symbolTableCount;
extern int IC;
extern int DC;

void addSymbol(char *label, int address, int isExternal, char *attributes);
int isLabel(char *line);
void processLine(char *line);
void firstPass(FILE *sourceFile);
void printSymbolTable();

#endif /* SYMBOL_TABLE_H */
