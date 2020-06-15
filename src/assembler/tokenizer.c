#include "assemble.h"
#include "assemble_instruction_utils.h"

/** Function which assigns mnemonic & func in the instr according to mnemonic and allocates memory
 * for the 'spine' of the array of strings (operands field) in instr and returns the number of operands
 *
 * @param mnemonic
 * @param operand
 * @param instr
 * @return number of operands
 */
int set_up_tokenized(char *mnemonic, instr_str_t *instr) {
    if (!strcmp(mnemonic, "add")) {
        instr->mnemonic = ADD;
        instr->assemble = assemble_dp;
        return 3;
    }
    if (!strcmp(mnemonic, "sub")) {
        instr->mnemonic = SUB;
        instr->assemble = assemble_dp;
        return 3;
    }
    if (!strcmp(mnemonic, "rsb")) {
        instr->mnemonic = RSB;
        instr->assemble = assemble_dp;
        return 3;
    }
    if (!strcmp(mnemonic, "and")) {
        instr->mnemonic = AND;
        instr->assemble = assemble_dp;
        return 3;
    }
    if (!strcmp(mnemonic, "eor")) {
        instr->mnemonic = EOR;
        instr->assemble = assemble_dp;
        return 3;
    }
    if (!strcmp(mnemonic, "orr")) {
        instr->mnemonic = ORR;
        instr->assemble = assemble_dp;
        return 3;
    }
    if (!strcmp(mnemonic, "andeq")) {
        instr->mnemonic = ANDEQ;
        instr->assemble = assemble_dp;
        return 3;
    }
    if (!strcmp(mnemonic, "mov")) {
        instr->mnemonic = MOV;
        instr->assemble = assemble_dp;
        return 2;
    }
    if (!strcmp(mnemonic, "tst")) {
        instr->mnemonic = TST;
        instr->assemble = assemble_dp;
        return 2;
    }
    if (!strcmp(mnemonic, "teq")) {
        instr->mnemonic = TEQ;
        instr->assemble = assemble_dp;
        return 2;
    }
    if (!strcmp(mnemonic, "cmp")) {
        instr->mnemonic = CMP;
        instr->assemble = assemble_dp;
        return 2;
    }
    if (!strcmp(mnemonic, "lsl")) {
        instr->mnemonic = LSL;
        instr->assemble = assemble_dp;
        return 2;
    }
    if (!strcmp(mnemonic, "mul")) {
        instr->mnemonic = MUL;
        instr->assemble = assemble_mult;
        return 3;
    }
    if (!strcmp(mnemonic, "mla")) {
        instr->mnemonic = MLA;
        instr->assemble = assemble_mult;
        return 4;
    }
    if (!strcmp(mnemonic, "ldr")) {
        instr->mnemonic = LDR;
        instr->assemble = assemble_sdt;
        return 2;
    }
    if (!strcmp(mnemonic, "str")) {
        instr->mnemonic = STR;
        instr->assemble = assemble_sdt;
        return 2;
    }
    if (mnemonic[0] == 'b') {
        if (!strcmp(mnemonic, "beq")) {
            instr->mnemonic = BEQ;
        }
        if (!strcmp(mnemonic, "bne")) {
            instr->mnemonic = BNE;
        }
        if (!strcmp(mnemonic, "bge")) {
            instr->mnemonic = BGE;
        }
        if (!strcmp(mnemonic, "blt")) {
            instr->mnemonic = BLT;
        }
        if (!strcmp(mnemonic, "ble")) {
            instr->mnemonic = BLE;
        }
        if (!strcmp(mnemonic, "b")) {
            instr->mnemonic = B;
        }
        instr->assemble = assemble_br;
        return 1;
    }
    perror("Error in finding mnemonic");
    exit(EXIT_FAILURE);
}

void tokenizer(program_t *prog) {
    instr_t *curr = prog->head->next;
    while (curr) {
        instr_str_t *instr = curr->instr_str;
        // Get the mnemonic
        char *mnemonic = strtok(instr->instr_line, " ");
        int num_operands = set_up_tokenized(mnemonic, instr);
        // Allocate array and assign operands into array
        instr->operands = calloc(num_operands, sizeof(char *));
        if (!instr->operands) {
            perror("Error allocating memory for operands array");
            exit(EXIT_FAILURE);
        } 
        for (int i = 0; i < num_operands - 1; i++) {
            instr->operands[i] = strtok(NULL, " ,");
        }
        instr->operands[num_operands - 1] = trim(strtok(NULL, ""));
        curr = curr->next;
    }
}
    
