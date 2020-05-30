#include <stdbool.h>
#include <stdint.h>

#ifndef ARM11_17_INSTRUCTIONS_H
#define ARM11_17_INSTRUCTIONS_H

typedef enum inst_type{
    DATA_PROC,
    MULT,
    DATA_TRANS,
    BRANCH,
    HALT
} inst_type;

/*
  The order of fields stored may be a bit messy,
  but this was done to save space
*/
typedef struct decoded_inst {
    inst_type type;

    // all: condition codes
    uint8_t cond;

    // DP, SDT, B
    uint16_t operand_offset;

    // DP : opcode
    uint8_t operation_code;

    // DP, SDT : indicate immediate or shifted register
    bool imm;

    // DP, M, SDT : registers
    int8_t rn;
    int8_t rd;
    // M : registers
    int8_t rs;
    int8_t rm;

    // SDT : P, U, L bits
    // pre/post indexing flag
    bool p;
    // up flag
    bool u;
    // load/store flag
    bool l;

    // DP, M : if to update CPSR flags
    bool set_cc;

    // M : if to multiply and accumulate
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

#endif //ARM11_17_INSTRUCTIONS_H
