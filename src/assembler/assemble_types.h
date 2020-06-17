#ifndef ARM11_17_ASSEMBLE_TYPES_H
#define ARM11_17_ASSEMBLE_TYPES_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include "../emulator/emulate_types.h"
#include "../emulator/emulate_instruction_utils.h"

/** Mnemonic type, corresponds to first word of instruction string
 * (for DP instruction types, set to same integer as for opcode enums
 *  in emulator for ease of conversion from mnemonic to opcode)
 */ 
typedef enum mnemonic_t {
  AND = 0,  
  EOR = 1,  
  SUB = 2, 
  RSB = 3,  
  ADD = 4,  
  TST = 8,  
  TEQ = 9,  
  CMP = 10, 
  ORR = 12, 
  MOV = 13,  
  MUL, 
  MLA, 
  LDR, 
  STR, 
  BEQ, 
  BNE, 
  BGE, 
  BLT, 
  BGT, 
  BLE, 
  B, 
  LSL, 
  ANDEQ
} mnemonic_t;

/************ LINKED LIST DATA STRUCTURES AND FUNCTIONS : ************/

/*********************** LABEL TO ADDRESS MAPPING ************************/

/** Assume as in emulator - 2^16 bytes of memory 
 *  ie addresses can be represented in 16 bits
 */ 
typedef struct symbol_table_elem_t {
  char *label;
  uint16_t address;
  struct symbol_table_elem_t *next;
} symbol_table_elem_t;

typedef struct symbol_table_t {
  symbol_table_elem_t *head;
  symbol_table_elem_t *tail;
} symbol_table_t;

symbol_table_t *create_symbol_table(void);
/* Adds label and its corresponding address into a symbol table */
void add_mapping(symbol_table_t *table, const char *label, uint16_t address);
/* Returns true and modifies address to value mapped by label iff found */
bool map(symbol_table_t *table, const char *label, uint16_t *address);
void free_symbol_table(symbol_table_t *table);

/*************************************************************************/

/************************** INSTRUCTIONS STORED **************************/

/** Instruction/data contained at an address in a program
 */ 
typedef struct prog_elem_t {
  uint16_t address;
  union {
    struct instr_str_t *instr_str; 
    uint32_t binary;
  };
  struct prog_elem_t *next;
} prog_elem_t, instr_t, data_t;

/** Program - a list of instructions/data : 
 * 
 *   | INSTRUCTIONS | DATA ADDED BY LOADER |
 *   ^              ^                      ^
 *  HEAD        LAST_INSTR                TAIL
 * 
 *  * last_instr marks the end of instructions to assemble,
 *    program will continue with data added by loading instructions (if any)
 */
typedef struct program_t {
  prog_elem_t *head;
  prog_elem_t *tail;
  prog_elem_t *last_instr; 
  prog_elem_t *curr;  // for looping over elements
} program_t;

/** An instruction string to be assembled, contains:
 *  * Original instruction string loaded from file
 *  * Instruction mnemonic type, assemble function
 *  * Instruction's operands as an array of pointers 
 *    to the original string
 */
typedef struct instr_str_t {
  char *instr_line;
  mnemonic_t mnemonic;
  void (*assemble)(program_t *, symbol_table_t *);
  char **operands;
} instr_str_t;

program_t *create_program(void);
/* Loads an instruction as an entire string at an address in the program*/
void add_instr(program_t *program, const char *instr_line, uint16_t address);
/* For LDR: Adds data to end of assembled program, returning its address */
uint16_t add_data(program_t *program, uint32_t binary_value); 
void free_program(program_t *program);

void free_instr_str(instr_str_t *instr_str);

/*************************************************************************/

#endif
