#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol.h"

void printTable(SymbolTable* table) {
    printf("Symbol Table: \n");

    for(int i = 0; i < table->count; i++) {
        printf("Name: %s, Type: %d\n", table->symbols[i].name, table->symbols[i].type);
    }
}


int main() {

    /*This just testing the symbol table for now :) using an array to store symbols just for
    simplification. this should work when we start working with a symbol table to implement the analysis phase
    of this language
    */

    SymbolTable table;
    Initialize(&table);

    AddSymbol(&table, "x", INTEGER);
    AddSymbol(&table, "y", FLOAT);
    AddSymbol(&table, "z", STRING);

    printTable(&table);

    UpdateSymbolValue(&table, "x", 10);

    Symbol* sym = LookUpSymbol(&table, "x");
    if(sym != NULL) {

        printf("Value: %d\n", sym->value.intValue);
    }

    return 0;
}