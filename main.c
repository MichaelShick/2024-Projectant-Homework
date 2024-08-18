#include "headers/step3.h"
#include "headers/macro.h"

/**
 * @brief Main function to process an assembly source file.
 *
 * This function reads an assembly source file, performs macro replacement, and then processes the file in two passes:
 * the first pass to build the symbol table and the second pass to generate the memory table. The results are written
 * to output files for symbols, memory, and macro-replaced text.
 *
 * @param argc The number of command-line arguments.
 * @param argv The command-line arguments, where `argv[1]` should be the path to the source file.
 * @return Returns 0 on successful execution, or 1 if an error occurs.
 */
int main(int argc, char *argv[]) {
    // Check if the correct number of arguments is provided
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <source file>\n", argv[0]);
        return 1;
    }

    // Attempt to open the source file
    FILE *sourceFile = fopen(argv[1], "r");
    if (sourceFile == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Read the entire source file into a string
    char *text = read_file(argv[1]);
    if (text == NULL) {
        fclose(sourceFile);
        return 1;
    }

    printf("Before macro replacement:================================================================\n\n");
    printf("%s\n\n", text);

    // Perform macro replacement and write the result to an output file
    printf("After macro replacement:================================================================\n\n");
    writeAllMacroToTextFile("files/output.am", text);
    char *result = read_file("files/output.am");
    if (result == NULL) {
        free(text);
        fclose(sourceFile);
        return 1;
    }

    printf("%s\n\n\n", result);

    // Create a copy of the result for the second pass
    char *text2 = (char*)malloc(strlen(result) + 1);
    if (text2 == NULL) {
        free(text);
        free(result);
        fclose(sourceFile);
        return 1;
    }
    strcpy(text2, result);

    // Perform the first pass to build the symbol table
    firstPass2(text);
    printSymbolTable();

    // Perform the second pass to generate the memory table
    secondPass(text2);

    // Write the memory table and entry symbols to output files
    writeMemoryTableToTextFile("files/output.ob");
    writeEntrySymbolsToTextFile("files/output.ent");

    // Free allocated memory and close the source file
    free(text);
    free(result);
    free(text2);
    fclose(sourceFile);

    return 0;
}
