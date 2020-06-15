#ifndef EMULATE_INSTRUCTION_UTILS_H
#define EMULATE_INSTRUCTION_UTILS_H

#include "emulate_types.h"

/* Mask to get top 2 bits of a word in GET_MS_N */
#define GET_MS_2 (0xC0000000) 
/* Masks to get bottom N bits of a word in GET_LS_N */
#define GET_LS_1 (0x1)
#define GET_LS_4 (0xF)
#define GET_LS_8 (0xFF)
#define GET_LS_12 (0xFFF)
#define GET_LS_24 (0xFFFFFF)
#define GET_LS_29 (0x1FFFFFFF)
#define GET_LS_30 (0x3FFFFFFF)
/* Mask which sign extends a negative 26 bit number, use with | */
#define NEG_SIGN_EXT_MASK (0xFC000000)

/* Exits program with unsuccessful termination and frees allocated memory */
void terminate(machine_state* ms);

/* Builds 32-bit word from 4 bytes stored in array (little endian) */
word_t build_nonzero_value(byte_t *ptr);

/* For debugging */
void print_bits(uint32_t x);

/* Loads word from memory at specified address */
uint32_t load_word(uint32_t address, machine_state *ms);
/* Stores word into memory at specified address */
void store_word(uint32_t address, machine_state *ms, uint32_t word);

/* Shifter type */ 
typedef enum shift_t {
  LSL_S = 0, // logical L
  LSR_S = 1, // logical R
  ASR_S = 2, // arithmetic R
  ROR_S = 3,  // rotate R
} shift_t;
/* Shifts an operand based on shift type and shift amount, updates carry */
uint32_t shifter(shift_t shift_type, uint32_t op, uint8_t shift, bool *carry);

/* DP only: Extracts operand as rotated immediate */
uint32_t imm_extract(uint16_t op, bool *carry);
/* DP and SDT: Extracts operand as shifted register contents */
uint32_t reg_extract(uint16_t op, machine_state* ms, bool *carry);

#endif
