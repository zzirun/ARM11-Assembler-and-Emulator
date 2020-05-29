#include <stdbool.h>
#include "types.h"


#ifndef UTILS_H
#define UTILS_H

uint32_t extractBits(uint32_t target, int start, int end);
void printBits(uint32_t x);
uint32_t buildInstruction(uint8_t* ptr);
uint32_t buildNonZeroValue(uint8_t* ptr);
<<<<<<< HEAD
=======
uint32_t load_word(uint32_t address, machine_state *ms);
void store_word(uint32_t address, machine_state *ms, uint32_t word);
>>>>>>> d3b1fa09ce775cc9d17632c716a7814a2a54a6ea
bool checkCond(machine_state *ms);
void execute(machine_state* ms);
uint32_t load_word(uint32_t address, machine_state *ms);
void store_word(uint32_t address, machine_state *ms, uint32_t word);
void output(machine_state* ms);

// shifter
typedef enum shiftType {
  LSL, // logical L
  LSR, // logical R
  ASR, // arithmetic R
  ROR  // rotate R
} shiftType;

uint32_t shifter(shiftType shiftT, uint32_t op, uint8_t shift, bool *carry);

// exits program with unsuccessful termination and frees allocated memory
void terminate(machine_state* ms);

#endif
