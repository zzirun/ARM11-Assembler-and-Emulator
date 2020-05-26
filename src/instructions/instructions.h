//
//

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

    //used for I, operand2, offset
    uint32_t operand_offset;

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

#endif //ARM11_17_INSTRUCTIONS_H
