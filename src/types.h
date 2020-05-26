//
// Created by Ivy Tam on 25/05/2020.
//
#include <stdint.h>
#include "instructions/instructions.h"
#ifndef ARM11_17_TYPES_H
#define ARM11_17_TYPES_H

enum inst_type{
    DATA_PROC,
    MULT,
    DATA_TRANS,
    BRANCH,
    HALT
};

typedef uint32_t register;
// type alias for a 32-bit register value
typedef struct {
    register regs[13];
    register pc;
    register cpsr;
    /* Regs 0-12 are general registers
     * There are actually 17 registers, register 13(SP) and 14 (LR) are ignored
       ignored for this exercise */
} registers;

typedef int8_t memory[65536];

typedef struct {
    instruction_type *instrToExecute;
    uint32_t instrToDecode;
    uint32_t instrFetched;
    bool fetch;
    registers registers;
    memory memory;
} machine_state;


#endif //ARM11_17_TYPES_H
