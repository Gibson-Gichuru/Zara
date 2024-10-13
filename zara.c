#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol.h"
#include "parser.h"

#define MAX_BUFFER_SIZE 4096 


char * ReadFileToString(const char* filename) {

    FILE* file = fopen(filename, "r");

    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char* content = (char*)malloc(MAX_BUFFER_SIZE);

    if(content == NULL) {
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }

    content[0] = '\0';

    char line[256];

    while(fgets(line, sizeof(line), file)) {
        strncat(content, line, MAX_BUFFER_SIZE - strlen(content) - 1);
    }

    fclose(file);

    return content;
}


/**
 * @brief Entry point of the Zara compiler
 *
 * This function is the entry point of the compiler. It should be responsible for
 * parsing the command line arguments, initializing the lexer, parser, and symbol
 * table, and driving the compilation process.
 */
int main(int argc, char* argv[]) {

    if(argc != 2) {
        printf("Usage: %s <source file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char* code = ReadFileToString(argv[1]);

    if(code == NULL) {
        perror("Error reading file");
        exit(EXIT_FAILURE);
    }
    Parser parser = InitParser(code);
    ParseProgram(&parser);

    printf("\nFinal Symbol Table:\n");
    DisplayTable(&parser.symbolTable);

    return 0;
}