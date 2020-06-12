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
typedef struct tokenized_instr_t {
  void (*assemble)(instr_t *);
  mnemonic_t mnemonic;
  char **operands;
} tokenized_instr_t;

/************ LINKED LIST DATA STRUCTURES AND FUNCTIONS : ************/

/************ LABEL TO ADDRESS MAPPING ************/

// assume as in emulator - 2^16 bytes of memory :
// addresses can be represented in 16 bits
typedef struct symbol_table_elem_t {
  const char *label;  
  uint16_t address;
  struct symbol_table_elem_t *next;
} symbol_table_elem_t;

typedef struct symbol_table_t {
  symbol_table_elem_t *head;
  symbol_table_elem_t *tail;
} symbol_table_t;

symbol_table_t *symbol_table_new();
void add_mapping(symbol_table_t *table, const char *label, uint16_t address);
bool map(symbol_table_t *table, const char *label, uint16_t *address);
void free_symbol_table(symbol_table_t *table);

/**************************************************/

/************************** INSTRUCTIONS STORED **************************/

// stores a single instr (as different formats) and address to next instr
// data in union replaced at each stage of assembly
typedef struct instr_t {
  union {
    char *instr_str;
    // RMB to free during tokenisation or implement using char[511]
    tokenized_instr_t tokenised_instr;
    decoded_instr_t decoded_instr;
    uint32_t binary_instr;
  };
  struct instr_t *next;
} instr_t;

typedef struct instr_list_t {
  instr_t *head;
  instr_t *tail;
} instr_list_t;

instr_list_t *instr_list_new();
void add_instr(instr_list_t *instr_list, const char *instr_str);
void free_instr_list(instr_list_t *instr_list);

/*************************************************************************/
