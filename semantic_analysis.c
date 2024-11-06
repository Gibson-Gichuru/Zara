#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SYMBOLS 100
#define MAX_SCOPE_LEVELS 10
#define MAX_FUNCTION_PARAMS 10

typedef struct {
    char name[50];
    char type[20];
} Symbol;

typedef struct {
    Symbol symbols[MAX_SYMBOLS];
    int count;
} Scope;

typedef struct {
    char name[50];
    char returnType[20];
    char paramTypes[MAX_FUNCTION_PARAMS][20];
    int paramCount;
} Function;

Scope scopes[MAX_SCOPE_LEVELS];
int scopeLevel = 0;

void enterScope() {
    if (scopeLevel < MAX_SCOPE_LEVELS - 1) {
        scopeLevel++;
        scopes[scopeLevel].count = 0;
    } else {
        printf("Max scope level reached.\n");
    }
}

void exitScope() {
    if (scopeLevel > 0) {
        scopeLevel--;
    } else {
        printf("No scope to exit.\n");
    }
}

void defineVariable(char *name, char *type) {
    strcpy(scopes[scopeLevel].symbols[scopes[scopeLevel].count].name, name);
    strcpy(scopes[scopeLevel].symbols[scopes[scopeLevel].count].type, type);
    scopes[scopeLevel].count++;
}

char* lookupVariable(char *name) {
    for (int i = scopeLevel; i >= 0; i--) {
        for (int j = 0; j < scopes[i].count; j++) {
            if (strcmp(scopes[i].symbols[j].name, name) == 0) {
                return scopes[i].symbols[j].type;
            }
        }
    }
    return NULL;
}

void checkTypeConsistency(char *varType, char *exprType) {
    if (strcmp(varType, exprType) != 0) {
        printf("Type Error: Cannot assign %s to %s.\n", exprType, varType);
    }
}

void checkVariable(char *name) {
    if (lookupVariable(name) == NULL) {
        printf("Name Error: Variable '%s' is not declared in the current scope.\n", name);
    }
}

void defineFunction(Function *functions, int *funcCount, char *name, char *returnType, char paramTypes[][20], int paramCount) {
    Function *func = &functions[*funcCount];
    strcpy(func->name, name);
    strcpy(func->returnType, returnType);
    func->paramCount = paramCount;
    for (int i = 0; i < paramCount; i++) {
        strcpy(func->paramTypes[i], paramTypes[i]);
    }
    (*funcCount)++;
}

void checkFunctionCall(Function *functions, int funcCount, char *name, char argTypes[][20], int argCount) {
    for (int i = 0; i < funcCount; i++) {
        if (strcmp(functions[i].name, name) == 0) {
            if (functions[i].paramCount != argCount) {
                printf("Type Error: Argument count mismatch for function '%s'.\n", name);
                return;
            }
            for (int j = 0; j < argCount; j++) {
                if (strcmp(functions[i].paramTypes[j], argTypes[j]) != 0) {
                    printf("Type Error: Argument type mismatch for function '%s'.\n", name);
                    return;
                }
            }
            return;
        }
    }
    printf("Function '%s' is not defined.\n", name);
}

void checkArrayConsistency(char *arrayType, char elementType[]) {
    if (strcmp(arrayType, elementType) != 0) {
        printf("Type Error: Array elements must be of the same type '%s'.\n", arrayType);
    }
}

int main() {
    Function functions[10];
    int funcCount = 0;
    
    // Test cases
    enterScope();

    // Declare variables
    defineVariable("x", "int");
    defineVariable("arr", "int_array");

    // Type checking example
    checkTypeConsistency("int", "float");  // Should show error

    // Checking variable scope
    checkVariable("y");  // Should show error

    // Array consistency check example
    checkArrayConsistency("int", "int");  // Should pass
    checkArrayConsistency("int", "float");  // Should show error
    
    // Function usage example
    char paramTypes1[2][20] = {"int", "int"};
    defineFunction(functions, &funcCount, "add", "int", paramTypes1, 2);
    char argTypes1[2][20] = {"int", "float"};
    checkFunctionCall(functions, funcCount, "add", argTypes1, 2);  // Should show error

    exitScope();
    return 0;
}
