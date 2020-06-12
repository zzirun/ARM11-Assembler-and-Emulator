#include "assemble.h"

uint32_t signed_to_twos_complement(int32_t value) {
    uint32_t result = abs(value);
    if (value < 0) {
        result = ~result + 1;
    }
    return result;
}

uint32_t assemble_br(tokenized_instr_t * token_i, symbol_table_t * symbolTable){
    // bits 24-27 is always 1010
    uint32_t result = 10 << 24;
    uint16_t label_address;
    char *label = token_i->operands[0];
    if(!map(symbolTable, label, label_address)){
        perror("Label does not exist");
        exit(EXIT_FAILURE);
    }
// instruction no in tokenized instr?
    uint32_t offset = ((int32_t)label_address - ((int32_t)instruction_no * 4) - 8) >> 2;
    // off by 8 bytes due to ARM pipeline
    offset = signed_to_twos_complement(offset) & 0xFFFFFF;
    // mask with 0xFFFFFF to get lower 24 bits
    result |= offset;
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



