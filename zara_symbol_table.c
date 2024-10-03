#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
  INTEGER,
  FLOAT,
  STRING,
  ARRAY,
  STACK,
} DataType;

typedef struct {
  char name[100];
  DataType type;
  union {
    int intValue;
    float floatValue;
    char stringValue[100];
  } value;

} Symbol;

typedef struct {
  Symbol symbols[100];
  int count;
} SymbolTable;

void initialize(SymbolTable *table) { table->count = 0; }

void addSymbol(SymbolTable *table, char *name, DataType type) {

  strcpy(table->symbols[table->count].name, name);
  table->symbols[table->count].type = type;
  table->count++;
}

Symbol *lookupSymbol(SymbolTable *table, char *name) {

  for (int i = 0; i < table->count; i++) {
    if (strcmp(table->symbols[i].name, name) == 0) {
      return &table->symbols[i];
    }
  }

  return NULL;
}

void updateSymbolValue(SymbolTable *table, char *name, int intValue) {

  Symbol *sym = lookupSymbol(table, name);

  if (sym != NULL) {
    sym->value.intValue = intValue;
  } else {
    printf("Symbol not found\n");
  }
}

void printTable(SymbolTable *table) {
  printf("Symbol Table: \n");

  for (int i = 0; i < table->count; i++) {
    printf("Name: %s, Type: %d\n", table->symbols[i].name,
           table->symbols[i].type);
  }
}

int main() {

  SymbolTable table;
  initialize(&table);
  /* Testing the Symbol Table.. should work fine once we have the analyzer working :)*/
  addSymbol(&table, "x", INTEGER);
  addSymbol(&table, "y", FLOAT);
  addSymbol(&table, "z", STRING);

  printTable(&table);

  updateSymbolValue(&table, "x", 10);

  Symbol *sym = lookupSymbol(&table, "x");
  if (sym != NULL) {

    printf("Value: %d\n", sym->value.intValue);
  }

  return 0;
}