#include <stdbool.h>
#include <stdio.h>
#include "types.h"


#ifndef UTILS_H
#define UTILS_H

//For debugging
void print_bits(uint32_t x);
//Binary loader
void bin_load(FILE *fp, uint8_t *array);
//Loads word from memory at specified address
uint32_t load_word(uint32_t address, machine_state *ms);
//Stores word into memory at specified address
void store_word(uint32_t address, machine_state *ms, uint32_t word);
//Checks condition against CPSR register
bool check_cond(machine_state *ms);
//Decodes instruction to be executed according to their types
void decode( machine_state* ms);
//Calls the different types of function during execution
void execute(machine_state* ms);
//Output at HALT
void output(machine_state* ms);
/*
  Shifter: returns shifted operand and updates carry
  Param: shiftT(ype), op(erand), shift(amount), *carry
*/
uint32_t shifter(shift_type shift_t, uint32_t op, uint8_t shift, bool *carry);
// exits program with unsuccessful termination and frees allocated memory
void terminate(machine_state* ms);

#endif
