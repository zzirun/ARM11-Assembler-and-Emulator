#ifndef ARM11_17_EMULATE_TYPES_H
#define ARM11_17_EMULATE_TYPES_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#define WORD_SIZE (32)
#define BYTE_SIZE (8)

#define ADDRESS_COUNT (65536)
#define REGISTER_COUNT (13)
#define PC_REG_NUM (15)

typedef uint8_t byte_t;
typedef uint32_t word_t;
typedef int8_t reg_add_t;

typedef uint8_t memory[ADDRESS_COUNT];

/**
 * Each register is 32 bits.
 * Regs 0-12 are general purpose registers (gpr).
 * There are actually 17 registers, 
 * register 13(SP) and 14 (LR) are ignored for this exercise.
 */
typedef struct registers {
  uint32_t gpr[REGISTER_COUNT];
  uint32_t pc;
  uint32_t cpsr;
} registers;

// 4 CPSR Flags : 31 - 28 : NZCV
typedef enum cpsr_flags {
  V = 1,
  C = 2,
  Z = 4,
  N = 8
} cpsr_flags;

typedef enum instr_type {
  DATA_PROC,
  MULT,
  DATA_TRANS,
  BRANCH,
  HALT
} instr_type;

// Instr condition codes
typedef enum cc {
  EQ = 0,  // check Z = 1
  NE = 1,  // check Z = 0
  GE = 10, // check N == V
  LT = 11, // check N != V
  GT = 12, // check Z == 0 && N == V
  LE = 13, // check Z == 1 || N != V
  AL = 14  // any
} cc;

// DP instr opcode representations
typedef enum opcode_t {
  AND_OP = 0,  // 0000 : a & b
  EOR_OP = 1,  // 0001 : a ^ b
  SUB_OP = 2,  // 0010 : a - b
  RSB_OP = 3,  // 0011 : b - a
  ADD_OP = 4,  // 0100 : a + b
  TST_OP = 8,  // 1000 : AND, no write
  TEQ_OP = 9,  // 1001 : EOR, no write
  CMP_OP = 10, // 1010 : SUB, no write
  ORR_OP = 12, // 1100 : a | b
  MOV_OP = 13  // 1101 : b
} opcode_t;

// Data Processing Instr Fields
typedef struct data_processing_t {
  // indicate immediate or shifted register for operand2
  bool imm;
  // opcode
  opcode_t opcode;
  // if to update CPSR flags
  bool set_cc;
  // registers
  reg_add_t rn;
  reg_add_t rd;
  // operand2
  uint16_t operand2;
} data_processing_t;

// Multiply Instr Fields
typedef struct multiply_t {
  // if to multiply and accumulate
  bool accum;
  // if to update CPSR flags
  bool set_cc;
  // registers
  reg_add_t rd;
  reg_add_t rn;
  reg_add_t rs;
  reg_add_t rm;
} multiply_t;

// Single Data Transfer Instr Fields
typedef struct single_data_transfer_t {
  // indicate immediate or shifted register for offset
  bool imm;
  // pre/post indexing flag
  bool p;
  // up flag
  bool u;
  // load/store flag
  bool l;
  // registers
  reg_add_t rn;
  reg_add_t rd;
  // offset
  uint16_t offset;
} single_data_transfer_t;

// Branch Instr Fields
typedef struct branch_t {
  // offset
  int32_t offset;
} branch_t;

// Decoded Instruction Format
typedef struct decoded_instr_t {
  // instr type
  instr_type type;
  // condition codes
  cc cond;
  // instr fields
  union {
    data_processing_t dp;
    multiply_t mul;
    single_data_transfer_t sdt;
    branch_t br;
  };

} decoded_instr_t;

typedef enum processor_state {
  EMPTY,
  FETCHED,
  DECODED
} processor_state;

typedef struct machine_state {
  memory mem;
  registers regs;
  word_t instr_fetched;
  decoded_instr_t instr_to_execute;
  processor_state ps;
} machine_state;

#endif //ARM11_17_TYPES_H
