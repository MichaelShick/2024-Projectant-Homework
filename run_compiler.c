#include <stdio.h>
#include <string.h>
#include "macro.h"
#include "symbol_table.h"
int main(int argc, char *argv[]) {
        if (argc != 2) {
            fprintf(stderr, "Usage: %s <source file>\n", argv[0]);
            return 1;
        }

        FILE *sourceFile = fopen(argv[1], "r");
        if (sourceFile == NULL) {
            perror("Error opening file");
            return 1;
        }

 char *text = read_file(argv[1]);
        printf("%s\n\n", text);
        printf("After macro replacement:================================================================\n\n");
        char *result = all_macro(text);
        printf("%s\n", result);


        firstPass(result);
        

        printSymbolTable();

        printf("IC: %d, DC: %d\n", IC, DC);

       

        return 0;
}
