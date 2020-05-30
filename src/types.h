//
// Created by Ivy Tam on 25/05/2020.
//
#include <stdint.h>
#include <stdbool.h>

#ifndef ARM11_17_TYPES_H
#define ARM11_17_TYPES_H

/*
  Each register is 32 bits.
  Regs 0-12 are general purpose registers (gpr).
  There are actually 17 registers, register 13(SP) and 14 (LR) are ignored
  ignored for this exercise.
*/

#define ADDRESS_COUNT 65536
#define REGISTER_COUNT 13
typedef uint8_t memory[ADDRESS_COUNT];

typedef struct registers{
  uint32_t gpr[REGISTER_COUNT];
  uint32_t PC;
  uint32_t CPSR;
} registers;

// 4 CPSR Flags : 31 - 28 : NZCV
typedef enum cpsrFlags {
  V = 1,
  C = 2,
  Z = 4,
  N = 8
} cpsrFlags;

// shifter
typedef enum shiftType {
  LSL, // logical L
  LSR, // logical R
  ASR, // arithmetic R
  ROR  // rotate R
} shiftType;

typedef enum inst_type{
    DATA_PROC,
    MULT,
    DATA_TRANS,
    BRANCH,
    HALT
} inst_type;

typedef struct decoded_inst {
    inst_type type;

    uint8_t cond;

    //used for branch offset
    int32_t branch_offset;

    //used for data processing and single data transfer
    uint16_t operand_offset;

    //opcode only in DP
    uint8_t operation_code;

    //For DP and SDT (To indicate immediate or shifted register)
    bool imm;

    //registers (only in DP, M, SDT)
    int8_t rn;
    int8_t rd;
    // Rs and Rm only in M
    int8_t rs;
    int8_t rm;

    //P, U and L only in SDT
    // pre/post indexing flag
    bool p;
    // up flag
    bool u;
    // load/store flag
    bool l;

    // only in DP and M (if to update CPSR flags)
    bool set_cc;

    // only in M (if to multiply and accumulate)
    bool accum;

} decoded_inst;

// instr condition codes
enum cc {
    EQ = 0,   // check Z = 1
    NE = 1,   // check Z = 0
    GE = 10,  // check N == V
    LT = 11,  // check N != V
    GT = 12,  // check Z == 0 && N == V
    LE = 13,  // check Z == 1 || N != V
    AL = 14   // any
};

// dp instr opcode representations
enum opcode {
    AND = 0,  // 0000 : a & b
    EOR = 1,  // 0001 : a ^ b
    SUB = 2,  // 0010 : a - b
    RSB = 3,  // 0011 : b - a
    ADD = 4,  // 0100 : a + b
    TST = 8,  // 1000 : AND, no write
    TEQ = 9,  // 1001 : EOR, no write
    CMP = 10, // 1010 : SUB, no write
    ORR = 12, // 1100 : a | b
    MOV = 13  // 1101 : b
};

typedef enum processor_state {
  EMPTY,
  FETCHED,
  DECODED
} processor_state;

typedef struct machine_state {
  memory mem;
  registers regs;
  uint32_t instrFetched;
  decoded_inst instrToExecute;
  processor_state ps;
} machine_state;


#endif //ARM11_17_TYPES_H
