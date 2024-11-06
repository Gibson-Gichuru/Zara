#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STACK_SIZE 100

typedef enum { SHIFT, REDUCE, ACCEPT, ERROR } ActionType;

typedef struct {
    ActionType action;
    int rule; // Rule number for reduce actions
} Action;

typedef struct {
    int stack[STACK_SIZE];
    int top;
} Stack;

Stack stateStack;
Stack symbolStack;

// Grammar rule structure
typedef struct {
    char lhs;
    char rhs[10];
} Rule;

// Define grammar rules
Rule grammarRules[] = {
    {'E', "E+E"},
    {'E', "E*E"},
    {'E', "(E)"},
    {'E', "id"},
    {'S', "ifEthenS"},
    {'S', "whileEdoS"},
    {'S', "{S}"},
    {'S', "id=E"}
};

int numRules = sizeof(grammarRules) / sizeof(grammarRules[0]);

// Action table example (simple table for demonstration)
Action actionTable[5][5] = {
    {{SHIFT, -1}, {ERROR, -1}, {ERROR, -1}, {SHIFT, -1}, {ERROR, -1}},
    {{ERROR, -1}, {REDUCE, 3}, {SHIFT, -1}, {ERROR, -1}, {ACCEPT, -1}},
    {{SHIFT, -1}, {REDUCE, 2}, {SHIFT, -1}, {ERROR, -1}, {ERROR, -1}},
    {{SHIFT, -1}, {REDUCE, 1}, {ERROR, -1}, {ERROR, -1}, {ERROR, -1}},
    {{REDUCE, 0}, {REDUCE, 0}, {REDUCE, 0}, {REDUCE, 0}, {ERROR, -1}}
};

// Initialize stacks
void initStack(Stack *stack) {
    stack->top = -1;
}

void push(Stack *stack, int value) {
    if (stack->top < STACK_SIZE - 1) {
        stack->stack[++stack->top] = value;
    } else {
        printf("Stack overflow.\n");
        exit(1);
    }
}

int pop(Stack *stack) {
    if (stack->top >= 0) {
        return stack->stack[stack->top--];
    } else {
        printf("Stack underflow.\n");
        exit(1);
    }
}

int top(Stack *stack) {
    if (stack->top >= 0) {
        return stack->stack[stack->top];
    } else {
        printf("Stack is empty.\n");
        exit(1);
    }
}

// Parsing functions
void shift(int state) {
    push(&stateStack, state);
    printf("Shifted to state %d\n", state);
}

void reduce(int rule) {
    Rule r = grammarRules[rule];
    int len = strlen(r.rhs);
    
    for (int i = 0; i < len; i++) {
        pop(&stateStack);
    }
    push(&stateStack, r.lhs);
    printf("Reduced by rule %c -> %s\n", r.lhs, r.rhs);
}

void parse(char *input) {
    initStack(&stateStack);
    push(&stateStack, 0); // Start state

    int i = 0;
    while (1) {
        int state = top(&stateStack);
        char symbol = input[i];

        Action action = actionTable[state][symbol - '0']; // For demo purposes only
        switch (action.action) {
            case SHIFT:
                shift(symbol - '0'); // Shift to next state
                i++;
                break;
            case REDUCE:
                reduce(action.rule); // Reduce by grammar rule
                break;
            case ACCEPT:
                printf("Input accepted.\n");
                return;
            case ERROR:
            default:
                printf("Syntax error at position %d.\n", i);
                return;
        }
    }
}

int main() {
    char input[] = "id+id*id";
    parse(input);
    return 0;
}
