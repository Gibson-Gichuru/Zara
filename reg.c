#include <stdio.h>
#include <stdlib.h>

#define NUM_REGISTERS 4  // Assume 4 physical registers for simplicity
#define MAX_VARIABLES 10 // Max number of variables for this example

typedef struct Variable {
    int id;
    int is_spilled;
    int register_num;
    int live_range_start;
    int live_range_end;
} Variable;

typedef struct Register {
    int occupied;
    int var_id;
} Register;

Register registers[NUM_REGISTERS];
Variable variables[MAX_VARIABLES];
int num_variables = 0;

/**
 * @brief Initializes all registers to be unoccupied and sets their variable IDs to -1.
 *
 * This function iterates over the array of registers and marks each register as unoccupied.
 * It also assigns a default variable ID of -1 to indicate that no variable is currently 
 * assigned to the register.
 */
void initialize_registers() {
    for (int i = 0; i < NUM_REGISTERS; i++) {
        registers[i].occupied = 0;
        registers[i].var_id = -1;
    }
}


/**
 * @brief Adds a variable to the table of variables
 *
 * This function takes the ID, start, and end of the live range of a variable as input
 * and adds the variable to the table of variables. The variable is initially marked
 * as not being spilled and not being assigned a register.
 *
 * @param id The unique identifier of the variable
 * @param start The start of the live range of the variable
 * @param end The end of the live range of the variable
 */
void add_variable(int id, int start, int end) {
    variables[num_variables].id = id;
    variables[num_variables].live_range_start = start;
    variables[num_variables].live_range_end = end;
    variables[num_variables].is_spilled = 0;
    variables[num_variables].register_num = -1;
    num_variables++;
}

/**
 * @brief Allocates a register to a variable
 *
 * This function takes the ID of a variable as input and allocates the first
 * available register to the variable. If all registers are occupied, the function
 * returns -1 to indicate that no register is available.
 *
 * @param var_id The ID of the variable to allocate a register
 * @return The number of the allocated register, or -1 if no register is available
 */
int allocate_register(int var_id) {
    for (int i = 0; i < NUM_REGISTERS; i++) {
        if (!registers[i].occupied) {
            registers[i].occupied = 1;
            registers[i].var_id = var_id;
            return i;
        }
    }
    return -1;  
}

/**
 * @brief Marks a variable as being spilled to memory
 *
 * This function takes the index of a variable in the table of variables
 * and marks the variable as being spilled to memory. A message is printed
 * to the console to indicate that the variable has been spilled to memory.
 *
 * @param var_index The index of the variable in the table of variables
 */
void spill_variable(int var_index) {
    variables[var_index].is_spilled = 1;
    printf("Variable %d is spilled to memory.\n", variables[var_index].id);
}

/**
 * @brief Performs register allocation for a set of variables
 *
 * This function takes no parameters and performs register allocation for all
 * variables in the table of variables. If a register is available, the function
 * assigns the register to the variable. If no register is available, the
 * function marks the variable as being spilled to memory.
 */
void register_allocation() {
    initialize_registers();

    for (int i = 0; i < num_variables; i++) {
        int reg_num = allocate_register(variables[i].id);

        if (reg_num != -1) {
            variables[i].register_num = reg_num;
            printf("Variable %d assigned to register %d\n", variables[i].id, reg_num);
        } else {
            spill_variable(i);
        }
    }
}

/**
 * @brief Frees a register previously allocated to a variable
 *
 * This function takes the ID of a variable as input and frees the register
 * previously allocated to the variable. If the variable is not currently
 * allocated a register, the function does nothing.
 *
 * @param var_id The ID of the variable to free the register for
 */
void free_register(int var_id) {
    for (int i = 0; i < NUM_REGISTERS; i++) {
        if (registers[i].var_id == var_id) {
            registers[i].occupied = 0;
            registers[i].var_id = -1;
            break;
        }
    }
}

int main() {
    
    add_variable(0, 0, 3);
    add_variable(1, 2, 5);
    add_variable(2, 1, 4);
    add_variable(3, 4, 6);
    
    register_allocation();

    for (int i = 0; i < num_variables; i++) {
        if (!variables[i].is_spilled) {
            free_register(variables[i].id);
            printf("Variable %d freed from register %d\n", variables[i].id, variables[i].register_num);
        }
    }

    return 0;
}
