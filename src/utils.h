#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "types.h"

// Exits program with unsuccessful termination and frees allocated memory
void terminate(machine_state* ms);

// Helper for output function in emulator
// builds 32-bit word from 4 bytes stored in array (little endian)
word_t build_nonzero_value(byte_t *ptr);

// For debugging
void print_bits(uint32_t x);

// Loads word from memory at specified address
uint32_t load_word(uint32_t address, machine_state *ms);
// Stores word into memory at specified address
void store_word(uint32_t address, machine_state *ms, uint32_t word);

// shifter type 
typedef enum shift_type {
  LSL_S = 0, // logical L
  LSR_S = 1, // logical R
  ASR_S = 2, // arithmetic R
  ROR_S = 3,  // rotate R
} shift_type;
// shifter - returns shifted operand and updates carry
// param: shiftT(ype), op(erand), shift(amount), *carry
uint32_t shifter(shift_type shift_t, uint32_t op, uint8_t shift, bool *carry);

// for DP, SDT : 
// extracts operand as rotated immediate
uint32_t imm_extract(uint16_t op, bool *carry);
// extracts operand as shifted register contents
uint32_t reg_extract(uint16_t op, machine_state* ms, bool *carry);

#endif
