#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "assembletypes.h"

void tokenizer(instr_list_t* i_l){
    instr_t *curr = i_l->head;
    while(!curr->next){
        tokenised_instr_t *token_i = (tokenised_instr_t *) calloc(1, sizeof(tokenised_instr_t));
        if(!token_i){
            perror("Error allocating memory for tokenized instruction");
            exit(EXIT_FAILURE);
        }
        // Get the mnemonic
        char *instr = strtok(curr->instr_str, " ");
        token_i->mnemonic_t = get_mnemonic(instr);
        //Get the operands of the instruction
        for(int i = 0; instr != NULL; i++){
            instr = strtok(NULL, ",");
            token_i->operands[i] = (char *) malloc(sizeof(instr));
            if(!token_i->operands[i]){
                perror("Error allocating memory for operand in tokenized instruction");
                exit(EXIT_FAILURE);
            }
            token_i->operands[i] = instr;
        }
        free(curr->instr_str);
        curr->tokenised_instr = token_i;
        curr = curr->next;
    }
}

mneomonic_t get_mnemonic(char *str){
    if(!strcmp(str, "add")){
        return ADD;
    }
    if(!strcmp(str, "sub")){
        return SUB;
    }
    if(!strcmp(str, "rsb")){
        return RSB;
    }
    if(!strcmp(str, "and")){
        return AND;
    }
    if(!strcmp(str, "eor")){
        return EOR;
    }
    if(!strcmp(str, "orr")){
        return ORR;
    }
    if(!strcmp(str, "mov")){
        return MOV;
    }
    if(!strcmp(str, "tst")){
        return TST;
    }
    if(!strcmp(str, "teq")){
        return TEQ;
    }
    if(!strcmp(str, "cmp")){
        return CMP;
    }
    if(!strcmp(str, "mul")){
        return MUL;
    }
    if(!strcmp(str, "mla")){
        return MLA;
    }
    if(!strcmp(str, "ldr")){
        return LDR;
    }
    if(!strcmp(str, "str")){
        return STR;
    }
    if(!strcmp(str, "beq")){
        return BEQ;
    }
    if(!strcmp(str, "bne")){
        return BNE;
    }
    if(!strcmp(str, "bge")){
        return BGE;
    }
    if(!strcmp(str, "blt")){
        return BLT;
    }
    if(!strcmp(str, "ble")){
        return BLE;
    }
    if(!strcmp(str, "b")){
        return B;
    }
    if(!strcmp(str, "lsl")){
        return LSL;
    }
    if(!strcmp(str, "andeq")){
        return ANDEQ;
    }

}