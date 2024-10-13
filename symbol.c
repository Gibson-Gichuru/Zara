#include "symbol.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Initializes a symbol table, setting the count of symbols to 0
 *
 * @param table The symbol table to initialize
 */
void InitSymbolTable(SymbolTable *table)
{

    table->count = 0;
}

/**
 * @brief Adds a symbol to the symbol table
 *
 * @param table The symbol table to add to
 * @param name The name of the symbol to add
 * @param type The type of the symbol to add
 * @param value The value of the symbol to add
 *
 * This function adds a symbol to the given symbol table. If the table is full or the
 * symbol already exists, it will exit the program. Otherwise, it will add the symbol
 * to the table and increment the count of symbols in the table.
 */
int AddSymbol(SymbolTable *table, char *name, DataType type, void *value)
{

    if (table->count >= 100)
    {
        printf("Symbol table is full\n");
        exit(1);
    }

    for (int i = 0; i < table->count; i++)
    {
        if (strcmp(table->symbols[i].name, name) == 0)
        {
            printf("Symbol already exists\n");
            exit(1);
        }
    }

    strcpy(table->symbols[table->count].name, name);
    table->symbols[table->count].type = type;

    switch (type)
    {
    case INTEGER:
        table->symbols[table->count].value.intValue = *(int *)value;
        break;
    case FLOAT:
        table->symbols[table->count].value.floatValue = *(float *)value;
        break;
    case STRING:
        strcpy(table->symbols[table->count].value.stringValue, (char *)value);
        break;
    case ARRAY:

    {
        int *arr = (int *)value; 
        for (int i = 0; arr[i] != 0; i++)
        {
            table->symbols[table->count].value.intValue = arr[i];
            table->count++; 
        }
    }

    default:
        break;
    }

    table->count++;
    return 0;
}

/**
 * @brief Look up a symbol in the symbol table by name
 *
 * @param table The symbol table to search
 * @param name The name of the symbol to look up
 * @return The symbol if found, or NULL if not found
 */
Symbol *LookUpSymbol(SymbolTable *table, const char *name)
{

    for (int i = 0; i < table->count; i++)
    {
        if (strcmp(table->symbols[i].name, name) == 0)
        {
            return &table->symbols[i];
        }
    }

    return NULL;
}

/**
 * @brief Update the value of a symbol in the symbol table
 *
 * @param table The symbol table to search
 * @param name The name of the symbol to update
 * @param value The new value of the symbol
 * @return 0 if successful, or an error code if not
 */
int UpdateSymbolValue(SymbolTable *table, char *name, void *value)
{

    Symbol *sym = LookUpSymbol(table, name);

    if (sym == NULL)
    {
        printf("Symbol not found\n");

        exit(1);
    }

    switch (sym->type)
    {
    case INTEGER:
        sym->value.intValue = *(int *)value;
        break;

    case FLOAT:

        sym->value.floatValue = *(float *)value;
        break;

    case STRING:

        strcpy(sym->value.stringValue, (char *)value);
        break;

    case ARRAY:
    {
        int *arr = (int *)value;
        for (int i = 0; arr[i] != 0; i++)
        {
            sym->value.intValue = arr[i];
        }
    }
    break;
    default:
        break;
    }

    return 0;
}

/**
 * @brief Prints the contents of a symbol table to stdout
 *
 * @param table The symbol table to print
 *
 * This function prints the contents of the given symbol table to stdout. It will
 * print the name, type, and value of each symbol in the table. The type and value
 * will be printed in a form appropriate for the type of the symbol.
 */
void DisplayTable(SymbolTable *table)
{
    for (int i = 0; i < table->count; i++)
    {
        printf("Name: %s, ", table->symbols[i].name);
        switch (table->symbols[i].type)
        {
        case INTEGER:
            printf("Type: INTEGER, Value: %d\n", table->symbols[i].value.intValue);
            break;
        case FLOAT:
            printf("Type: FLOAT, Value: %.2f\n", table->symbols[i].value.floatValue);
            break;
        case STRING:
            printf("Type: STRING, Value: %s\n", table->symbols[i].value.stringValue);
            break;
        case ARRAY:
            printf("Type: ARRAY, Value: ");
            for (int j = 0; table->symbols[i].value.intArray[j] != 0; j++)
            {
                printf("%d ", table->symbols[i].value.intArray[j]);
            }
            printf("\n");
            break;
        }
    }
}