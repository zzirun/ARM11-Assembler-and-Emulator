//
//

#ifndef ARM11_17_INSTRUCTIONS_H
#define ARM11_17_INSTRUCTIONS_H
#include "types.h"
#include <stdbool.h>

typedef struct {
    enum inst_type type;
    uint8_t cond;

    //used for I, operand2, offset
    uint32_t immediate_operand;

    //opcode only in DP
    uint8_t operation_code;

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

};

#endif //ARM11_17_INSTRUCTIONS_H
