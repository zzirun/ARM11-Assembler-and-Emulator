#include "assemble.h"
uint32_t assemble_br(tokenized_instr_t * token_i, symbol_table_t * symbolTable){
    // bits 24-27 is always 1010
    uint32_t result = 10 << 24;
    // set the condition codes (cc)
    switch(token_i->mnemonic){
        case BEQ:
            break; // cc : 0000
        case BNE:
            result |= 1 << 28; // cc : 0001
            break;
        case BGE:
            result |= 10 << 28; // cc : 1010
            break;
        case BLT:
            result |= 11 << 28; // cc : 1011
            break;
        case BGT:
            result |= 12 << 28; // cc : 1100
            break;
        case BLE:
            result |= 13 << 28; // cc : 1101
            break;
        case B:
            result |= 14 << 28; // cc : 1110
            break;
        default:
            assert(false);
    }

}



