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
    for (; *s; s++) {
        if (c == *s) {
            return 1;
        }
    }
    return 0;
}

/** Function which assigns mnemonic & func in the token_i according to mnemonic and allocates memory
 * for the 'spine' of the array of strings (operands field) in token_i and returns the number of operands
 *
 * @param mnemonic
 * @param operand
 * @param token_i
 * @return number of operands
 */
int get_num_operands(char *mnemonic, tokenised_instr_t *token_i) {
    char *str = trim_whitespace(mnemonic);
    //
    if (!strcmp(str, "add") || !strcmp(str, "sub") || !strcmp(str, "rsb") || !strcmp(str, "and") || !strcmp(str, "eor")
        || !strcmp(str, "orr")) {
        if (!strcmp(str, "add")) {
            token_i->mnemonic_t = ADD;
        }
        if (!strcmp(str, "sub")) {
            token_i->mnemonic_t = SUB;
        }
        if (!strcmp(str, "rsb")) {
            token_i->mnemonic_t = RSB;
        }
        if (!strcmp(str, "and")) {
            token_i->mnemonic_t = AND;
        }
        if (!strcmp(str, "eor")) {
            token_i->mnemonic_t = EOR;
        }
        if (!strcmp(str, "orr")) {
            token_i->mnemonic_t = ORR;
        }
        if (!strcmp(str, "andeq")) {
            token_i->mnemonic_t = ANDEQ;
        }
        token_i->func = assemble_dp;
        token_i->operands = (char **) malloc(3 * sizeof(char *));
        if (!token_i->operands) {
            perror("Error allocating memory for operands array");
            exit(EXIT_FAILURE);
        }
        return 3;
    }
    if (!strcmp(str, "mov") || !strcmp(str, "tst") || !strcmp(str, "teq") || !strcmp(str, "cmp") ||
        !strcmp(str, "lsl")) {
        if (!strcmp(str, "mov")) {
            token_i->mnemonic_t = MOV;
        }
        if (!strcmp(str, "tst")) {
            token_i->mnemonic_t = TST;
        }
        if (!strcmp(str, "teq")) {
            token_i->mnemonic_t = TEQ;
        }
        if (!strcmp(str, "cmp")) {
            token_i->mnemonic_t = CMP;
        }
        if (!strcmp(str, "lsl")) {
            token_i->mnemonic_t = LSL;
        }
        token_i->func = assemble_dp;
        token_i->operands = (char **) malloc(2 * sizeof(char *));
        if (!token_i->operands) {
            perror("Error allocating memory for operands array");
            exit(EXIT_FAILURE);
        }
        return 2;
    }
    if (!strcmp(str, "mul")) {
        token_i->mnemonic_t = MUL;
        token_i->func = mult_assembly;
        token_i->operands = (char **) malloc(3 * sizeof(char *));
        if (!token_i->operands) {
            perror("Error allocating memory for operands array");
            exit(EXIT_FAILURE);
        }
        return 3;
    }
    if (!strcmp(str, "mla")) {
        token_i->mnemonic_t = MLA;
        token_i->func = mult_assembly;
        token_i->operands = (char **) malloc(4 * sizeof(char *));
        if (!token_i->operands) {
            perror("Error allocating memory for operands array");
            exit(EXIT_FAILURE);
        }
        return 4;
    }
    if (!stcmp(str, "ldr") || !strcmp(str, "str")) {
        if (!strcmp(str, "ldr")) {
            token_i->mnemonic_t = LDR;
        }
        if (!strcmp(str, "str")) {
            token_i->mnemonic_t = STR;
        }
        token_i->func = assemble_sdt;
        token_i->operands = (char **) malloc(2 * sizeof(char *));
        if (!token_i->operands) {
            perror("Error allocating memory for operands array");
            exit(EXIT_FAILURE);
        }
        return 2;
    }
    if (str[0] == 'b') {
        if (!strcmp(str, "beq")) {
            token_i->mnemonic_t = BEQ;
        }
        if (!strcmp(str, "bne")) {
            token_i->mnemonic_t = BNE;
        }
        if (!strcmp(str, "bge")) {
            token_i->mnemonic_t = BGE;
        }
        if (!strcmp(str, "blt")) {
            token_i->mnemonic_t = BLT;
        }
        if (!strcmp(str, "ble")) {
            token_i->mnemonic_t = BLE;
        }
        if (!strcmp(str, "b")) {
            token_i->mnemonic_t = B;
        }
        token_i->func = assemble_br;
        token_i->operands = (char **) malloc(1 * sizeof(char *));
        if (!token_i->operands) {
            perror("Error allocating memory for operands array");
            exit(EXIT_FAILURE);
        }
        return 1;
    }
    perror("Error in finding mnemonic");
    exit(EXIT_FAILURE);
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
        int num_operands = get_num_operands(instr, token_i);
        for (int i = 0; i < num_operands - 1; i++) {
                instr = strtok(NULL, ",");
                token_i->operands[i] = (char *) malloc(sizeof(instr));
                if (!operands[i]) {
                    perror("Error allocating memory for string in operands");
                    exit(EXIT_FAILURE);
                }
                token_i->operands[i] = trim_whitespace(instr);
            }
        instr = strtok(NULL, "\n");
        token_i->operands[num_operands - 1] = instr;
        }
        curr->tokenised_instr = token_i;
        curr = curr->next;
    }
}
