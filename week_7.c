#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INSTRUCTIONS 100

// Structure to represent an instruction in TAC
typedef struct {
    char op[10];
    char arg1[10];
    char arg2[10];
    char result[10];
} TACInstruction;

TACInstruction instructions[MAX_INSTRUCTIONS];
int instructionCount = 0;
int tempVarCount = 0;

// Function to create a temporary variable name
void newTemp(char *temp) {
    sprintf(temp, "t%d", tempVarCount++);
}

// Function to emit a TAC instruction
void emit(char *op, char *arg1, char *arg2, char *result) {
    TACInstruction instr;
    strcpy(instr.op, op);
    strcpy(instr.arg1, arg1);
    strcpy(instr.arg2, arg2);
    strcpy(instr.result, result);
    instructions[instructionCount++] = instr;
}

// Generate TAC for an arithmetic expression
void generateExpression(char *result, char *arg1, char *op, char *arg2) {
    newTemp(result);
    emit(op, arg1, arg2, result);
}

// Generate TAC for an assignment statement
void generateAssignment(char *var, char *expr) {
    emit("=", expr, "", var);
}

// Generate TAC for if statement
void generateIf(char *cond, char *labelTrue, char *labelFalse) {
    emit("if", cond, "goto", labelTrue);
    emit("goto", "", "", labelFalse);
}

// Generate TAC for labels
void generateLabel(char *label) {
    emit("label", "", "", label);
}

// Generate TAC for a while loop
void generateWhile(char *cond, char *labelStart, char *labelEnd) {
    emit("ifFalse", cond, "goto", labelEnd);
}

// Print TAC instructions
void printTAC() {
    printf("Three-Address Code (TAC):\n");
    for (int i = 0; i < instructionCount; i++) {
        if (strcmp(instructions[i].op, "label") == 0) {
            printf("%s:\n", instructions[i].result);
        } else if (strcmp(instructions[i].arg2, "") == 0) {
            printf("%s = %s %s %s\n", instructions[i].result, instructions[i].op, instructions[i].arg1, instructions[i].arg2);
        } else {
            printf("%s = %s %s %s\n", instructions[i].result, instructions[i].arg1, instructions[i].op, instructions[i].arg2);
        }
    }
}

// Testing function
void testTAC() {
    char temp1[10], temp2[10], temp3[10];
    
    // Example 1: Assignment
    generateAssignment("a", "5");

    // Example 2: Arithmetic Expression
    generateExpression(temp1, "a", "+", "b");
    generateAssignment("c", temp1);

    // Example 3: If statement
    generateIf("a > b", "L1", "L2");
    generateLabel("L1");
    generateAssignment("d", "1");
    generateLabel("L2");

    // Example 4: While loop
    generateLabel("L3");
    generateWhile("a < 10", "L3", "L4");
    generateExpression(temp2, "a", "+", "1");
    generateAssignment("a", temp2);
    generateLabel("L4");

    // Print TAC
    printTAC();
}

int main() {
    testTAC();
    return 0;
}
