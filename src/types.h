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

typedef struct registers{
    uint32_t gpr[13];
    uint32_t PC;
    uint32_t CPSR;
} registers;


typedef struct machine_state {
  bool fetched;
  uint32_t instrFetched;
  uint32_t instrToDecode;
  decoded_inst instrToExecute;
} machine_state;


#endif //ARM11_17_TYPES_H
