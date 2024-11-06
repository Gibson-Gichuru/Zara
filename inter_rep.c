#include <stdio.h>
#include <string.h>


typedef struct {
    char result[10];
    char arg1[10];
    char op[3];
    char arg2[10];
} TAC;

/**
 * Populate a TAC instruction with the given result, argument 1, operator, and argument 2.
 * @param instruction The TAC instruction to populate.
 * @param result The result of the instruction.
 * @param arg1 The first argument of the instruction.
 * @param op The operator of the instruction.
 * @param arg2 The second argument of the instruction.
 */
void generateTAC(TAC *instruction, const char *result, const char *arg1, const char *op, const char *arg2) {
    strcpy(instruction->result, result);
    strcpy(instruction->arg1, arg1);
    strcpy(instruction->op, op);
    strcpy(instruction->arg2, arg2);
}

/**
 * Translates an arithmetic expression into Three-Address Code (TAC).
 *
 * This function generates TAC instructions for the expression (a + b) * (c - d).
 * It uses temporary variables to hold intermediate results and populates the 
 * provided TAC array with instructions.
 *
 * @param tacArray The array to store generated TAC instructions.
 * @param index Pointer to the current index in the TAC array. It will be 
 *              incremented as instructions are added.
 * @param a The first operand for addition.
 * @param b The second operand for addition.
 * @param c The first operand for subtraction.
 * @param d The second operand for subtraction.
 */
void translateExpression(TAC *tacArray, int *index, const char *a, const char *b, const char *c, const char *d) {
    char temp1[10] = "t1";
    char temp2[10] = "t2";
    char temp3[10] = "t3";
    
    generateTAC(&tacArray[(*index)++], temp1, a, "+", b); 
    generateTAC(&tacArray[(*index)++], temp2, c, "-", d);   
    generateTAC(&tacArray[(*index)++], temp3, temp1, "*", temp2); 
}


// Example: if (x > 0) goto L1 else goto L2
void translateIfStatement(TAC *tacArray, int *index, const char *condition, const char *trueLabel, const char *falseLabel) {
    generateTAC(&tacArray[(*index)++], "if", condition, "goto", trueLabel); 
    generateTAC(&tacArray[(*index)++], "goto", falseLabel, "", "");          
}

void translateMethodCall(TAC *tacArray, int *index, const char *methodName, const char *param) {
    generateTAC(&tacArray[(*index)++], "call", methodName, param, ""); 
}

int main() {
    TAC tacArray[100];
    int index = 0; 

    printf("Generating TAC for (a + b) * (c - d):\n");
    translateExpression(tacArray, &index, "a", "b", "c", "d");

    printf("\nGenerating TAC for if statement:\n");
    translateIfStatement(tacArray, &index, "x > 0", "L1", "L2");
    printf("\nGenerating TAC for method call:\n");
    translateMethodCall(tacArray, &index, "print", "x");
    printf("\nGenerated TAC:\n");
    for (int i = 0; i < index; i++) {
        printf("%s = %s %s %s\n", tacArray[i].result, tacArray[i].arg1, tacArray[i].op, tacArray[i].arg2);
    }

    return 0;
}
