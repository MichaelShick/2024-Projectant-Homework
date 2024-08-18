#include "../headers/step3.h"

/**
 * @brief Global memory table to store memory words for the assembler.
 */
memory memorytable[8192];

/**
 * @brief Retrieves the opcode corresponding to a given function name.
 *
 * This function maps the provided assembly instruction (e.g., "mov", "add") to its corresponding opcode.
 *
 * @param func The function name (e.g., "mov", "add").
 * @return The opcode corresponding to the function name, or -1 if the function is not recognized.
 */
int getopcodebyfunc(char* func) {
    if(strcmp(func,"mov")==0) {
        return 1;
    }
    if(strcmp(func,"cmp")==0) {
        return 1 << 1;
    }
    if(strcmp(func,"add")==0) {
        return 1 << 2;
    }
    if(strcmp(func,"sub")==0) {
        return 1 << 3;
    }
    if(strcmp(func,"not")==0) {
        return 1 << 4;
    }
    if(strcmp(func,"clr")==0) {
        return 1 << 5;
    }
    if(strcmp(func,"lea")==0) {
        return 1 << 6;
    }
    if(strcmp(func,"inc")==0) {
        return 1 << 7;
    }
    if(strcmp(func,"dec")==0) {
        return 1 << 8;
    }
    if(strcmp(func,"jmp")==0) {
        return 1 << 9;
    }
    if(strcmp(func,"bne")==0) {
        return 1 << 10;
    }
    if(strcmp(func,"red")==0) {
        return 1 << 11;
    }
    if(strcmp(func,"prn")==0) {
        return 1 << 12;
    }
    if(strcmp(func,"jsr")==0) {
        return 1 << 13;
    }
    if(strcmp(func,"rts")==0) {
        return 1 << 14;
    }
    if(strcmp(func,"stop")==0) {
        return 1 << 15;
    }
    return -1;
}

/**
 * @brief Writes a number to the memory table at the specified line.
 *
 * This function takes a 20-bit number and writes it into the memory table across five 4-bit fields (A, B, C, D, E).
 *
 * @param number The number to write to the memory table.
 * @param numline The line number in the memory table where the number should be written.
 */
void writeToMemoryTable(int number, int numline) {
    memorytable[numline].A = (number >> 0) & 0xF;
    memorytable[numline].B = (number >> 4) & 0xF;
    memorytable[numline].C = (number >> 8) & 0xF;
    memorytable[numline].D = (number >> 12) & 0xF;
    memorytable[numline].E = (number >> 16) & 0xF;
}

/**
 * @brief Handles directives starting with a dot (.) such as .data, .string, .entry, .extern.
 *
 * This function processes lines that start with a dot (.) and writes the appropriate data to the memory table.
 *
 * @param line The line containing the directive.
 * @param lineNum A pointer to the current line number in the memory table.
 */
void hundledot(char* line, int *lineNum) {
    char* lineRest = line;
    char* val;
    line++;
    if(strcmp(line,"data")==0) {
        // Write to data
        val = strtok_r(lineRest, " ,\t", &lineRest);
        while(val != NULL) {
            writeToMemoryTable(atoi(val), *lineNum);
            (*lineNum)++;
        }
    } else if(strcmp(line,"string")==0) {
        // Write to string
        val = strtok_r(lineRest, " ,\t", &lineRest);
        while(val != NULL) {
            for (int tav = 0; tav < strlen(val); tav++) {
                writeToMemoryTable(val[tav], *lineNum);
                (*lineNum)++;
            }
            writeToMemoryTable(0, *lineNum); // Null terminator
            (*lineNum)++;
        }
    } else if(strcmp(line,"entry")==0) {
        // Handle entry (not implemented in this function)
        return;
    } else if(strcmp(line,"extern")==0) {
        // Handle extern (not implemented in this function)
        return;
    }
    return;
}

/**
 * @brief Checks if the given command is a label.
 *
 * This function checks if the provided command ends with a colon, indicating that it is a label.
 *
 * @param command The command to check.
 * @return 1 if the command is a label, 0 otherwise.
 */
int islabel(char* command) {
    if(command[strlen(command) - 1] == ':') {
        return 1;
    }
    return 0;
}

/**
 * @brief Writes the opcode corresponding to a label to the memory table.
 *
 * This function removes the colon from the label, retrieves the opcode using the label name, and writes it to the memory table.
 *
 * @param func The label name (without colon).
 * @param lineNum A pointer to the current line number in the memory table.
 */
void writeopcodebylabel(char* func, int *lineNum) {
    func[strlen(func) - 1] = '\0';
    writeToMemoryTable(getopcodebyfunc(func), 0);
    (*lineNum)++;
}

/**
 * @brief Checks if the function requires two operands.
 *
 * This function checks if the provided function name corresponds to an instruction that requires two operands.
 *
 * @param func The function name to check.
 * @return 1 if the function requires two operands, 0 otherwise.
 */
int is2oppfunc(char* func) {
    if(strcmp(func,"mov") == 0 || strcmp(func,"cmp") == 0 || strcmp(func,"add") == 0 || strcmp(func,"sub") == 0) {
        return 1;
    }
    return 0;
}

/**
 * @brief Retrieves the function number corresponding to a function name.
 *
 * This function maps the provided assembly instruction (e.g., "add", "sub") to its corresponding function number.
 *
 * @param func The function name (e.g., "add", "sub").
 * @return The function number corresponding to the function name, or -1 if the function is not recognized.
 */
int getnumbyfunc(char* func) {
    if(strcmp(func,"add") == 0) {
        return 10;
    }
    if(strcmp(func,"sub") == 0) {
        return 11;
    }
    if(strcmp(func,"clr") == 0) {
        return 10;
    }
    if(strcmp(func,"not") == 0) {
        return 11;
    }
    if(strcmp(func,"inc") == 0) {
        return 12;
    }
    if(strcmp(func,"dec") == 0) {
        return 13;
    }
    if(strcmp(func,"jmp") == 0) {
        return 10;
    }
    if(strcmp(func,"bne") == 0) {
        return 11;
    }
    if(strcmp(func,"jsr") == 0) {
        return 12;
    }
    return -1;
}

/**
 * @brief Checks if the provided string represents a register.
 *
 * This function checks if the provided string starts with 'r', indicating that it represents a register.
 *
 * @param reg The string to check.
 * @return 1 if the string represents a register, 0 otherwise.
 */
int isreg(char* reg) {
    if(reg[0] == 'r') {
        return 1;
    }
    return 0;
}

/**
 * @brief Retrieves the register number from the register string.
 *
 * This function extracts the register number from the provided register string.
 *
 * @param reg The register string (e.g., "r1", "r2").
 * @return The register number as an integer.
 */
int getnumbyreg(char* reg) {
    return atoi(reg + 1);
}

/**
 * @brief Checks if the provided string represents an array.
 *
 * This function checks if the provided string starts with '[', indicating that it represents an array.
 *
 * @param arr The string to check.
 * @return 1 if the string represents an array, 0 otherwise.
 */
int isarr(char* arr) {
    if(arr[0] == '[') {
        return 1;
    }
    return 0;
}

/**
 * @brief Writes an array's address to the memory table.
 *
 * This function writes the address of an array to the memory table.
 *
 * @param arr The array string (e.g., "[arrayName]").
 * @param lineNum A pointer to the current line number in the memory table.
 */
void wrtitememobyarr(char* arr, int *lineNum) {
    char* arrname;
    arrname = strtok(arr + 1,"[");
    writeToMemoryTable(findSymbolAddress(arrname), *lineNum);
    (*lineNum)++;
}

/**
 * @brief Writes a label's address to the memory table.
 *
 * This function writes the address of a label to the memory table.
 *
 * @param label The label name.
 * @param lineNum A pointer to the current line number in the memory table.
 */
void wrtitememobylable(char* label, int *lineNum) {
    writeToMemoryTable(findSymbolAddress(label), *lineNum);
    (*lineNum)++;
}

/**
 * @brief Checks if the function requires one operand.
 *
 * This function checks if the provided function name corresponds to an instruction that requires one operand.
 *
 * @param func The function name to check.
 * @return 1 if the function requires one operand, 0 otherwise.
 */
int is1oppfunc(char* func) {
    if(strcmp(func,"not") == 0 || strcmp(func,"clr") == 0 || strcmp(func,"inc") == 0 || strcmp(func,"dec") == 0 ||
       strcmp(func,"jmp") == 0 || strcmp(func,"bne") == 0 || strcmp(func,"red") == 0 || strcmp(func,"prn") == 0 ||
       strcmp(func,"jsr") == 0 || strcmp(func,"rts") == 0) {
        return 1;
    }
    return 0;
}

/**
 * @brief Determines the addressing mode of the first operand.
 *
 * This function determines the addressing mode of the first operand based on its prefix.
 *
 * @param op1 The first operand.
 * @return The addressing mode as an integer.
 */
int wayofopaddr_op1(char* op1) {
    if(op1[0] == '#') {
        return 0;
    }
    if(op1[0] == 'r') {
        return 1;
    }
    if(op1[0] == '[') {
        return 2;
    }
    return 3 << 6;
}

/**
 * @brief Determines the addressing mode of the second operand.
 *
 * This function determines the addressing mode of the second operand based on its prefix.
 *
 * @param op2 The second operand.
 * @return The addressing mode as an integer.
 */
int wayofopaddr_op2(char* op2) {
    if(op2[0] == '#') {
        return 0;
    }
    if(op2[0] == 'r') {
        return 1 << 6;
    }
    if(op2[0] == '[') {
        return 2 << 6;
    }
    return 3 << 6;
}

/**
 * @brief Checks if the function requires a function number.
 *
 * This function checks if the provided function name corresponds to an instruction that requires a function number.
 *
 * @param func The function name to check.
 * @return 1 if the function requires a function number, 0 otherwise.
 */
int isneedfuncnum(char* func) {
    if(strcmp(func,"add") == 0 || strcmp(func,"sub") == 0 || strcmp(func,"clr") == 0 || strcmp(func,"not") == 0 ||
       strcmp(func,"inc") == 0 || strcmp(func,"dec") == 0 || strcmp(func,"jmp") == 0 || strcmp(func,"bne") == 0 ||
       strcmp(func,"jsr") == 0) {
        return 1;
    }
    return 0;
}

/**
 * @brief Checks if the provided string represents an immediate value.
 *
 * This function checks if the provided string starts with '#', indicating that it represents an immediate value.
 *
 * @param num The string to check.
 * @return 1 if the string represents an immediate value, 0 otherwise.
 */
int isnum(char* num) {
    if(num[0] == '#') {
        return 1;
    }
    return 0;
}

/**
 * @brief Writes an immediate value to the memory table.
 *
 * This function writes the provided immediate value to the memory table.
 *
 * @param num The immediate value string (e.g., "#5").
 * @param lineNum A pointer to the current line number in the memory table.
 */
void writememmoynum(char* num, int *lineNum) {
    writeToMemoryTable(atoi(num + 1), *lineNum);
    (*lineNum)++;
}

/**
 * @brief Performs the second pass of the assembler on the source file content.
 *
 * This function processes each line of the source file content during the second pass, encoding instructions
 * and writing them to the memory table.
 *
 * @param sourceFileContent The content of the source file.
 */
void secondPass(char *sourceFileContent) {
    char line[100];
    char *token;
    char *rest = sourceFileContent;
    int lineNum = 100; // Start at initial instruction counter

    while ((token = strtok_r(rest, "\n", &rest))) {
        strncpy(line, token, 100 - 1);
        line[100 - 1] = '\0';

        // Ignore comments, empty lines, and directives
        if (line[0] == ';' || line[0] == '\n' || line[0] == '\0') continue;
        if(line[0] == '.') {
           hundledot(line, &lineNum);
            continue;
        }

        // Parse command and operands
        char *func;
        char *lineRest = line;
        char* command = strtok_r(lineRest, " \t", &lineRest);

        if(islabel(command)){
            func = strtok_r(lineRest, " \t", &lineRest);
        } else {
            func = command;
        }

        writeToMemoryTable(getopcodebyfunc(func), lineNum++);
        if(is2oppfunc(func)) {
            int funcnum = isneedfuncnum(func) ? getnumbyfunc(func) : 0;
            funcnum = funcnum << 12;
            int memo = 0;
            char* op1 = strtok_r(lineRest, " \t,", &lineRest);
            memo |= wayofopaddr_op1(op1);
            char* op2 = strtok_r(lineRest, " \t,", &lineRest);
            memo |= wayofopaddr_op2(op2);
            memo = memo | funcnum;
            int regnum;
            if(isreg(op1)) {
               regnum = getnumbyreg(op1);
               regnum = regnum << 2;
               memo |= regnum;
            } else {
                if(isarr(op1)) {
                    wrtitememobyarr(op1, &lineNum);
                } else if(isnum(op1)) {
                    writememmoynum(op1, &lineNum);
                } else {
                    wrtitememobylable(op1, &lineNum);
                }
            }
            if(isreg(op2)) {
                regnum = getnumbyreg(op2);
                regnum = regnum << 8;
                memo |= regnum;
            } else {
                if(isarr(op2)) {
                    wrtitememobyarr(op2, &lineNum);
                } else if(isnum(op2)) {
                    writememmoynum(op2, &lineNum);
                } else {
                    wrtitememobylable(op2, &lineNum);
                }
            }
            writeToMemoryTable(memo, lineNum);
            lineNum++;
            continue;
        } else if(is1oppfunc(func)) {
            int funcnum = isneedfuncnum(func) ? getnumbyfunc(func) : 0;
            funcnum = funcnum << 12;
            int memo = 0;
            char* op1 = strtok_r(lineRest, " \t,", &lineRest);
            memo |= wayofopaddr_op1(op1);
            memo = memo | funcnum;
            int regnum;
            if(isreg(op1)) {
                regnum = getnumbyreg(op1);
                regnum = regnum << 2;
                memo |= regnum;
            } else {
                if(isarr(op1)) {
                    wrtitememobyarr(op1, &lineNum);
                } else if(isnum(op1)) {
                    writememmoynum(op1, &lineNum);
                } else {
                    wrtitememobylable(op1, &lineNum);
                }
            }
            writeToMemoryTable(memo, lineNum);
            lineNum++;
            continue;
        } else {
            continue;
        }
    }
}

/**
 * @brief Prints the contents of the memory table to the console.
 *
 * This function iterates over the memory table and prints each memory word's contents in hexadecimal format.
 */
void printMemoryTable() {
    for (int i = 100; i < 8192; i++) {
        printf("MEMORY[%d]: A%X-B%X-C%X-D%X-E%X\n",
               i, memorytable[i].E, memorytable[i].D, memorytable[i].C,
               memorytable[i].B, memorytable[i].A);
    }
}

/**
 * @brief Writes the contents of the memory table to a binary file.
 *
 * This function writes each memory word in the memory table to a binary file as a sequence of bytes.
 *
 * @param filename The name of the binary file to write to.
 */
void writeMemoryTableToBinaryFile(const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    for (int i = 100; i < 8192; i++) {
        unsigned char data[5];
        data[0] = memorytable[i].A;
        data[1] = memorytable[i].B;
        data[2] = memorytable[i].C;
        data[3] = memorytable[i].D;
        data[4] = memorytable[i].E;
        fwrite(data, sizeof(unsigned char), 5, file);
    }

    fclose(file);
}

/**
 * @brief Writes the contents of the memory table to a text file.
 *
 * This function writes each memory word in the memory table to a text file in a human-readable format.
 *
 * @param filename The name of the text file to write to.
 */
void writeMemoryTableToTextFile(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    for (int i = 100; i < 8192; i++) {
        fprintf(file, "MEMORY[%d]: A%X-B%X-C%X-D%X-E%X\n",
                i, memorytable[i].A, memorytable[i].B, memorytable[i].C,
                memorytable[i].D, memorytable[i].E);
    }

    fclose(file);
}
