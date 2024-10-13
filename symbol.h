#ifndef symbol_h
#define symbol_h

#define MAX_SYMBOLS 1000
#define MAX_NAME_LENGTH 100
#define MAX_ARRAY_LENGTH 100

typedef enum {
    INTEGER,
    FLOAT,
    STRING,
    ARRAY,
    STACK,
} DataType;

typedef struct 
{
    /* data */
    char name[MAX_NAME_LENGTH];
    DataType type;
    union 
    {
        int intValue;
        float floatValue;
        char stringValue[MAX_NAME_LENGTH];
        int intArray[MAX_ARRAY_LENGTH];
    } value;
    
} Symbol;

typedef struct {
    Symbol symbols[100];
    int count;
} SymbolTable;


void InitSymbolTable(SymbolTable* table);

int AddSymbol(SymbolTable* table, char* name, DataType type, void* value);

Symbol* LookUpSymbol(SymbolTable* table, const char* name);

int UpdateSymbolValue(SymbolTable* table, char* name, void* value);

void DisplayTable(SymbolTable* table);

#endif