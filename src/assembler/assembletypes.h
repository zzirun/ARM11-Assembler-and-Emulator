#ifndef ARM11_17_ASSEMBLER
#define ARM11_17_ASSEMBLER

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include "../types.h"

// instr mnemonic (first word of instr)
// for DP, set to same integer as opcode enum -
// - for ease of conversion of mnemonic to opcode
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

// instr split into string operands
typedef struct instr_str_t {
  char *instr_line;
  mnemonic_t mnemonic;
  void (*assemble)(program_t *, symbol_table_t *);
  char **operands;
} instr_str_t;

void free_instr_str(instr_str_t *);

/************ LINKED LIST DATA STRUCTURES AND FUNCTIONS : ************/

/************ LABEL TO ADDRESS MAPPING ************/

// assume as in emulator - 2^16 bytes of memory :
// addresses can be represented in 16 bits
typedef struct symbol_table_elem_t {
  char *label;
  uint16_t address;
  symbol_table_elem_t *next;
} symbol_table_elem_t;

typedef struct symbol_table_t {
  symbol_table_elem_t *head;
  symbol_table_elem_t *tail;
} symbol_table_t;

symbol_table_t *create_symbol_table(void);
void add_mapping(symbol_table_t *table, const char *label, uint16_t address);
bool map(symbol_table_t *table, const char *label, uint16_t *address);
void free_symbol_table(symbol_table_t *table);

/**************************************************/

/************************** INSTRUCTIONS STORED **************************/

// stores a single instr (as different formats) and address to next instr
// data in union replaced at each stage of assembly
typedef struct prog_elem_t {
  uint16_t address;
  union {
    instr_str_t *instr_str; 
    uint32_t binary;
  };
  instr_t *next;
} prog_elem_t, instr_t, data_t;

typedef struct program_t {
  prog_elem_t *head;
  prog_elem_t *tail;
  prog_elem_t *last_instr;
  prog_elem_t *curr; // for looping over elements
} program_t;

program_t *create_program(void);
void add_instr(program_t *program, const char *instr_line, uint16_t address);
uint16_t add_data(program_t *program, uint32_t binary_value); //for ldr in sdt
void free_program(program_t *program);

/*************************************************************************/

#endif
