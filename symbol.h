#ifndef symbol_h
#define symbol_h
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
    char name[100];
    DataType type;
    union 
    {
        int intValue;
        float floatValue;
        char stringValue[100];
    } value;
    
} Symbol;

typedef struct {
    Symbol symbols[100];
    int count;
} SymbolTable;


void Initialize(SymbolTable* table);

void AddSymbol(SymbolTable* table, char* name, DataType type);

Symbol* LookUpSymbol(SymbolTable* table, char* name);

void UpdateSymbolValue(SymbolTable* table, char* name, int intValue);

#endif