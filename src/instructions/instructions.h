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

    //used for branch offset
    int32_t branch_offset;

    //used for data processing and single data transfer
    uint16_t operand_offset;

    uint8_t cond;

    //opcode only in DP
    uint8_t operation_code;

    //For DP and SDT (To indicate immediate or shifted register)
    bool I;

    //registers (only in DP, M, SDT)
    int8_t Rn;
    int8_t Rd;
    // Rs and Rm only in M
    int8_t Rs;
    int8_t Rm;

    //P, U and L only in SDT
    bool P;
    bool U;
    bool L;

    // S only in DP and M
    bool S;

    // A only in M
    bool A;

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

// execution for diff instr types
void dataProc(machine_state *ms);
void mult(machine_state *ms);
void single_data_transfer(machine_state *ms);
void branch(machine_state *ms);

#endif //ARM11_17_INSTRUCTIONS_H
