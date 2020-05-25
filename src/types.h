//
// Created by Ivy Tam on 25/05/2020.
//
#include <stdint.h>
#ifndef ARM11_17_TYPES_H
#define ARM11_17_TYPES_H

#define PC regs[13]
#define CPSR regs[14]

enum inst_type{
    DATA_PROC,
    MULT,
    DATA_TRANS,
    BRANCH,
    HALT
};

typedef uint32_t Register;
// type alias for a 32-bit register value
typedef struct Registers{
    Register regs[15];
    /* Regs 0-12 are general registers
     * There are actually 17 registers, register 13(SP) and 14 (LR) are ignored
       ignored for this exercise */
};

#endif //ARM11_17_TYPES_H
