//
// Created by Zhai Zirun on 11/6/20.
//

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "../types.h"

word_t sdt(tokenized_instr_t tokenized_instr, word_t *extra_words, int max_instr, int curr_instr, int extra_words_size) {
    decoded_instr_t instr = {
            .type = DATA_TRANS;
            .cond = AL;
            .sdt.u = 1;
            .sdt.rd = str_to_reg_add(tokenized_instr.operands[1]);
            .sdt.imm = 0;
    };

    if (get_mnemonic(tokenized_instr.operands[0])== LDR) {
        //loads from memory to register
        //requires L bit to be set
        instr.sdt.l = 1;
    } else {
        //str, requires L bit to be clear
        instr.sdt.l = 0;
    }

    if(tokenized_instr.operands[2][0] == '=') {
        //in form <=expression>

        word_t expression = str_to_imm(tokenized_instr.operands[2][1]);
        //value of expression

        if (expression <= 0xFF) {
            //value of exp fits into argument of mov
            //TODO: compile instr as a mov instead of an ldr
        } else {
            //put value of expression at end of assembled program
            instr.sdt.p = 1;
            instr.sdt.rn = 15; //register number of PC
            instr.sdt.imm = (((max_lines + extra_words_size) - current_line) << 2) - 8;
            //calculated offset
            //TODO: add expression to extra_words
        }

    } else {
        if (tokenized_instr.operands[4][0] == ']') {
            //post-indexing addressing specification
            if(tokenized_instr.operands[5][0] == '#') {
                //[Rn],<#expression>
                instruction.rn = str_to_reg_add(tokenized_instr.operands[3]); //TODO: implement helper fn
                instruction.sdt.imm = str_to_imm(tokenized_instr.operands[5][1]); //TODO: implement helper fn
            } else {
                //TODO: [Rn],{+/-}Rm{,<shift>} (OPTIONAL)
            }

        } else {
            //pre-indexed address specification
            //[Rn]:
            instr.sdt.p = 1;
            instr.sdt.rn = str_to_reg_add(tokenized_instr.operands[3]);
            if ([Rn, <#expression>]) {
                //[Rn, <#expression>]
                 word_t exp = str_to_imm(tokenized_instr.operands[4][1]);
                 instruction.sdt.imm = abs(exp); //TODO: check if the abs is correct when applied to word
                 instruction.flag_2 = (value >= 0);
            } else {
                //TODO: [Rn,{+/-}Rm{,<shift>}] (OPTIONAL)
            }
        }
    }

    return sdt_encode(instr);
}

word_t sdt_encode(decoded_instr_t instr) {
    //TODO
}

