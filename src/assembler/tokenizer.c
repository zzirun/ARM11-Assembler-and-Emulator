#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "assemble.h"

/** Function which assigns mnemonic & func in the token_i according to mnemonic and allocates memory
 * for the 'spine' of the array of strings (operands field) in token_i and returns the number of operands
 *
 * @param mnemonic
 * @param operand
 * @param token_i
 * @return number of operands
 */
int set_up_tokenized(char *mnemonic, tokenized_instr_t *token_i) {
    if (!strcmp(mnemonic, "add")) {
        token_i->mnemonic = ADD;
        token_i->assemble = assemble_dp;
        return 3;
    }
    if (!strcmp(mnemonic, "sub")) {
        token_i->mnemonic = SUB;
        token_i->assemble = assemble_dp;
        return 3;
    }
    if (!strcmp(mnemonic, "rsb")) {
        token_i->mnemonic = RSB;
        token_i->assemble = assemble_dp;
        return 3;
    }
    if (!strcmp(mnemonic, "and")) {
        token_i->mnemonic = AND;
        token_i->assemble = assemble_dp;
        return 3;
    }
    if (!strcmp(mnemonic, "eor")) {
        token_i->mnemonic = EOR;
        token_i->assemble = assemble_dp;
        return 3;
    }
    if (!strcmp(mnemonic, "orr")) {
        token_i->mnemonic = ORR;
        token_i->assemble = assemble_dp;
        return 3;
    }
    if (!strcmp(mnemonic, "andeq")) {
        token_i->mnemonic = ANDEQ;
        token_i->assemble = assemble_dp;
        return 2;
    }
    if (!strcmp(mnemonic, "mov")) {
        token_i->mnemonic = MOV;
        token_i->assemble = assemble_dp;
        return 2;
    }
    if (!strcmp(mnemonic, "tst")) {
        token_i->mnemonic = TST;
        token_i->assemble = assemble_dp;
        return 2;
    }
    if (!strcmp(mnemonic, "teq")) {
        token_i->mnemonic = TEQ;
        token_i->assemble = assemble_dp;
        return 2;
    }
    if (!strcmp(mnemonic, "cmp")) {
        token_i->mnemonic = CMP;
        token_i->assemble = assemble_dp;
        return 2;
    }
    if (!strcmp(mnemonic, "lsl")) {
        token_i->mnemonic = LSL;
        token_i->assemble = assemble_dp;
        return 2;
    }
    if (!strcmp(mnemonic, "mul")) {
        token_i->mnemonic = MUL;
        token_i->assemble = mult_assembly;
        return 3;
    }
    if (!strcmp(mnemonic, "mla")) {
        token_i->mnemonic = MLA;
        token_i->assemble = mult_assembly;
        return 4;
    }
    if (!strcmp(mnemonic, "ldr")) {
        token_i->mnemonic = LDR;
        token_i->assemble = assemble_sdt;
        return 2;
    }
    if (!strcmp(mnemonic, "str")) {
        token_i->mnemonic = STR;
        token_i->assemble = assemble_sdt;
        return 2;
    }
    if (mnemonic[0] == 'b') {
        if (!strcmp(mnemonic, "beq")) {
            token_i->mnemonic = BEQ;
        }
        if (!strcmp(mnemonic, "bne")) {
            token_i->mnemonic = BNE;
        }
        if (!strcmp(mnemonic, "bge")) {
            token_i->mnemonic = BGE;
        }
        if (!strcmp(mnemonic, "blt")) {
            token_i->mnemonic = BLT;
        }
        if (!strcmp(mnemonic, "ble")) {
            token_i->mnemonic = BLE;
        }
        if (!strcmp(mnemonic, "b")) {
            token_i->mnemonic = B;
        }
        token_i->assemble = assemble_br;
        return 1;
    }
    perror("Error in finding mnemonic");
    exit(EXIT_FAILURE);
}

void tokenizer(instr_list_t *i_l) {
    instr_t *curr = i_l->head->next;
    while (curr) {
        tokenized_instr_t *token_i = (tokenized_instr_t *) calloc(1, sizeof(tokenized_instr_t));
        if (!token_i) {
            perror("Error allocating memory for tokenized instruction");
            exit(EXIT_FAILURE);
        }
        // Get the mnemonic
        char *mnemonic = strtok(curr->instr_str, " ");
        int num_operands = set_up_tokenized(mnemonic, token_i);
        // Allocate array and assign operands into array
        token_i->operands = calloc(num_operands, sizeof(char *));
        if (!token_i->operands) {
            perror("Error allocating memory for operands array");
            exit(EXIT_FAILURE);
        } 
        for (int i = 0; i < num_operands - 1; i++) {
        token_i->operands[i] = strtok(NULL, " ,");
        }
        token_i->operands[num_operands - 1] = strtok(NULL, "");
        curr->tokenized_instr = token_i;
        curr = curr->next;
    }  
}
    
