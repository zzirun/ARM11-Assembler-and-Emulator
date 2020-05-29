//
// Created by Ivy Tam on 25/05/2020.
//
#include <stdint.h>
#include <stdbool.h>
#include "instructions/instructions.h"

#ifndef ARM11_17_TYPES_H
#define ARM11_17_TYPES_H

/*
  Each register is 32 bits.
  Regs 0-12 are general purpose registers (gpr).
  There are actually 17 registers, register 13(SP) and 14 (LR) are ignored
  ignored for this exercise.
*/
#define ADDRESS_COUNT 65536
typedef uint8_t memory[ADDRESS_COUNT];

typedef struct registers{
  uint32_t gpr[13];
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
  //uint32_t instrToDecode;
  //bool decode;
} machine_state;

#endif //ARM11_17_TYPES_H
