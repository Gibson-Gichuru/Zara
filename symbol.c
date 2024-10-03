#include "symbol.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void Initialize(SymbolTable* table) {

    table->count = 0;
}

void AddSymbol(SymbolTable* table, char* name, DataType type) {

    strcpy(table->symbols[table->count].name, name);
    table->symbols[table->count].type = type;
    table->count++;
}

Symbol* LookUpSymbol(SymbolTable* table, char* name) {
    
    for(int i = 0; i < table->count; i++) {
        if(strcmp(table->symbols[i].name, name) == 0) {
            return &table->symbols[i];
        }
    }

    return NULL;
}


void UpdateSymbolValue(SymbolTable* table, char* name, int intValue){

    Symbol* sym = LookUpSymbol(table, name);

    if(sym != NULL){
        sym->value.intValue = intValue;
    }else {
        printf("Symbol not found\n");
    }
}