#include "assemble.h"

uint32_t signed_to_twos_complement(int32_t value) {
    uint32_t result = abs(value);
    if (value < 0) {
        result = ~result + 1;
    }
    return result;
}

void assemble_br(instr_list_t *il, symbol_table_t *symbol_table){
    tokenized_instr_t *token_i = il->curr->tokenized_instr;
    // bits 24-27 is always 1010
    uint32_t bin_instr = 10 << 24;

    uint16_t label_address;
    char *label = token_i->operands[0];
    if(!map(symbol_table, label, &label_address)){
        perror("Label does not exist");
        exit(EXIT_FAILURE);
    }
    uint32_t offset = ((int32_t)label_address - (int32_t)il->curr->address - 8) >> 2;
    // off by 8 bytes due to ARM pipeline
    offset = signed_to_twos_complement(offset) & 0xFFFFFF;
    // mask with 0xFFFFFF to get lower 24 bits
    bin_instr |= offset;

    // set the condition codes (cc)
    switch(token_i->mnemonic){
        case BEQ:
            break; // cc : 0000
        case BNE:
            bin_instr |= NE << 28; // cc : 0001
            break;
        case BGE:
            bin_instr |= GE << 28; // cc : 1010
            break;
        case BLT:
            bin_instr |= LT << 28; // cc : 1011
            break;
        case BGT:
            bin_instr |= GT << 28; // cc : 1100
            break;
        case BLE:
            bin_instr |= LE << 28; // cc : 1101
            break;
        case B:
            bin_instr |= AL << 28; // cc : 1110
            break;
        default:
            assert(false);
    }
    il->curr->binary_instr = bin_instr;
}



