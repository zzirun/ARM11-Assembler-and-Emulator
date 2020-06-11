#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "assembletypes.h"

// This function removes whitespace in the front and back of a string
char *trim_whitespace(char *str) {
    char *end;
    while (*str == ' ') {
        str++;
    }
    end = str + strlen(str) - 1;
    while (end > str && *end == ' ') {
        end--;
    }
    end[1] = '\0';
    return str;
}
//This function checks if a character is in a string
int contains(char *s, char c) {
    for(; *s; s++){
        if(c == *s){
            return 1;
        }
    }
    return 0;
}
mneomonic_t get_mnemonic(char *str) {
    if (!strcmp(str, "add")) {
        return ADD;
    }
    if (!strcmp(str, "sub")) {
        return SUB;
    }
    if (!strcmp(str, "rsb")) {
        return RSB;
    }
    if (!strcmp(str, "and")) {
        return AND;
    }
    if (!strcmp(str, "eor")) {
        return EOR;
    }
    if (!strcmp(str, "orr")) {
        return ORR;
    }
    if (!strcmp(str, "mov")) {
        return MOV;
    }
    if (!strcmp(str, "tst")) {
        return TST;
    }
    if (!strcmp(str, "teq")) {
        return TEQ;
    }
    if (!strcmp(str, "cmp")) {
        return CMP;
    }
    if (!strcmp(str, "mul")) {
        return MUL;
    }
    if (!strcmp(str, "mla")) {
        return MLA;
    }
    if (!strcmp(str, "ldr")) {
        return LDR;
    }
    if (!strcmp(str, "str")) {
        return STR;
    }
    if (!strcmp(str, "beq")) {
        return BEQ;
    }
    if (!strcmp(str, "bne")) {
        return BNE;
    }
    if (!strcmp(str, "bge")) {
        return BGE;
    }
    if (!strcmp(str, "blt")) {
        return BLT;
    }
    if (!strcmp(str, "ble")) {
        return BLE;
    }
    if (!strcmp(str, "b")) {
        return B;
    }
    if (!strcmp(str, "lsl")) {
        return LSL;
    }
    if (!strcmp(str, "andeq")) {
        return ANDEQ;
    }

}

void tokenizer(instr_list_t *i_l) {
    instr_t *curr = i_l->head;
    while (!curr) {
        tokenised_instr_t *token_i = (tokenised_instr_t *) calloc(1, sizeof(tokenised_instr_t));
        if (!token_i) {
            perror("Error allocating memory for tokenized instruction");
            exit(EXIT_FAILURE);
        }
        // Get the mnemonic
        char *instr = strtok(curr->instr_str, " ");
        token_i->mnemonic_t = get_mnemonic(instr);
        //Get the operands of the instruction
        for (int i = 0;; i++) {
            instr = strtok(NULL, ",");
            if (instr == NULL) {
                break;
            }
            //to deal with operands like [r1, #28]
            if (contains(instr, '[') && !contains(instr, ']')) {
                token_i->operands[i] = (char *) malloc(100);
                if (!token_i->operands[i]) {
                    perror("Error allocating memory for operand in tokenized instruction");
                    exit(EXIT_FAILURE);
                }
                do {
                    strcat(token_i->operands[i], trim_whitespace(instr));
                    char *comma = ",";
                    strcat(token_i->operands[i], comma);
                    instr = strtok(NULL, ",");
                } while (!contains(instr, ']'));
                strcat(token_i->operands[i], trim_whitespace(instr));
            } else {
                operands[i] = (char *) malloc(sizeof(instr));
                if (!operands[i]) {
                    perror("Error allocating memory for operand in tokenized instruction");
                    exit(EXIT_FAILURE);
                }
                token->operands[i] = trim_whitespace(instr);
            }
        }
        free(curr->instr_str);
        curr->tokenised_instr = token_i;
        curr = curr->next;
    }
}
