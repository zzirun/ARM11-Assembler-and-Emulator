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

int num_of_operands(char *str) {
    int result = 1;
    bool memory = false;
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == ',' && !memory) {
            result++;
        }
        if (str[i] == '[') {
            memory = true;
        }
        if (str[i] == ']') {
            memory = false;
        }
    }
    return result;
}
/** Function which assigns mnemonic & func in the token_i according to mnemonic and allocates memory
 * for the 'spine' of the array of strings (operands field) in token_i
 *
 * @param mnemonic
 * @param operand
 * @param token_i
 * @return number of operands
 */
int set_up_tokenised(char *mnemonic, char *operand, tokenised_instr_t *token_i) {
    char *str = trim_whitespace(mnemonic);
    int operands_num = num_of_operands(operand);
    token_i->operands = (char **) malloc(operands_num * sizeof(char *));
    if(!token_i->operands){
        perror("Error allocating memory for operands array");
        exit(EXIT_FAILURE);
    }
    if (!strcmp(str, "add")){
        token_i->mnemonic_t = ADD;
        token_i->func = assemble_dp;
        return operands_num;
    }
    if (!strcmp(str, "sub")) {
        token_i->mnemonic_t = SUB;
        token_i->func = assemble_dp;
        return operands_num;
    }
    if (!strcmp(str, "rsb")) {
        token_i->mnemonic_t = RSB;
        token_i->func = assemble_dp;
        return operands_num;
    }
    if (!strcmp(str, "and")) {
        token_i->mnemonic_t = AND;
        token_i->func = assemble_dp;
        return operands_num;
    }
    if (!strcmp(str, "eor")) {
        token_i->mnemonic_t = EOR;
        token_i->func = assemble_dp;
        return operands_num;
    }
    if (!strcmp(str, "orr")) {
        token_i->mnemonic_t = ORR;
        token_i->func = assemble_dp;
        return operands_num;
    }
    if (!strcmp(str, "mov")) {
        token_i->mnemonic_t = MOV;
        token_i->func = assemble_dp;
        return operands_num;
    }
    if (!strcmp(str, "tst")) {
        token_i->mnemonic_t = TST;
        token_i->func = assemble_dp;
        return operands_num;
    }
    if (!strcmp(str, "teq")) {
        token_i->mnemonic_t = TEQ;
        token_i->func = assemble_dp;
        return operands_num;
    }
    if (!strcmp(str, "cmp")) {
        token_i->mnemonic_t = CMP;
        token_i->func = assemble_dp;
        return operands_num;
    }
    if (!strcmp(str, "mul")) {
        token_i->mnemonic_t = MUL;
        token_i->func = mult_assembly;
        return operands_num;
    }
    if (!strcmp(str, "mla")) {
        token_i->mnemonic_t = MLA;
        token_i->func = mult_assembly;
        return operands_num;
    }
    if (!strcmp(str, "ldr")) {
        token_i->mnemonic_t = LDR;
        token_i->func = assemble_sdt;
        return operands_num;
    }
    if (!strcmp(str, "str")) {
        token_i->mnemonic_t = STR;
        token_i->func = assemble_sdt;
        return operands_num;
    }
    if (!strcmp(str, "beq")) {
        token_i->mnemonic_t = BEQ;
        token_i->func = assemble_br;
        return operands_num;
    }
    if (!strcmp(str, "bne")) {
        token_i->mnemonic_t = BNE;
        token_i->func = assemble_br;
        return operands_num;
    }
    if (!strcmp(str, "bge")) {
        token_i->mnemonic_t = BGE;
        token_i->func = assemble_br;
        return operands_num;
    }
    if (!strcmp(str, "blt")) {
        token_i->mnemonic_t = BLT;
        token_i->func = assemble_br;
        return operands_num;
    }
    if (!strcmp(str, "ble")) {
        token_i->mnemonic_t = BLE;
        token_i->func = assemble_br;
        return operands_num;
    }
    if (!strcmp(str, "b")) {
        token_i->mnemonic_t = B;
        token_i->func = assemble_br;
        return operands_num;
    }
    if (!strcmp(str, "lsl")) {
        token_i->mnemonic_t = LSL;
        token_i->func = assemble_dp;
        return operands_num;
    }
    if (!strcmp(str, "andeq")) {
        token_i->mnemonic_t = ANDEQ;
        token_i->func = assemble_dp;
        return operands_num;
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
        char* instr_cpy;
        strcpy(instr_cpy, curr->instr_str);
        char *operands = strtok(instrcpy,"\n");
        char *mnemonic = strtok(curr->instr_str, " ");
        int num_operands = set_up_tokenized(mnemonic, operands, token_i);
        for (int i = 0; i < num_operands; i++) {
            operands = strtok(NULL, ",");
            //to deal with operands like [r1, #28]
            if (contains(instr, '[') && !contains(instr, ']')) {
                // I allocated 100 in the heap but unsure of a good size for these operands
                token_i->operands[i] = (char *) malloc(100);
                if (!token_i->operands[i]) {
                    perror("Error allocating memory for string in operands");
                    exit(EXIT_FAILURE);
                }
                do {
                    strcat(token_i->operands[i], trim_whitespace(operands));
                    char *comma = ",";
                    strcat(token_i->operands[i], comma);
                    instr = strtok(NULL, ",");
                } while (!contains(operands, ']'));
                strcat(token_i->operands[i], trim_whitespace(operands));
            } else {
                operands[i] = (char *) malloc(sizeof(operands));
                if (!operands[i]) {
                    perror("Error allocating memory for string in operands");
                    exit(EXIT_FAILURE);
                }
                token->operands[i] = trim_whitespace(operands);
            }
        }
        free(curr->instr_str);
        curr->tokenised_instr = token_i;
        curr = curr->next;
    }
}
