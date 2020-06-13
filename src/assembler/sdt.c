//
// Created by Zhai Zirun on 11/6/20.
//

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "../types.h"
#include "assembletypes.h"
#include "assemble_utils.h"

word_t sdt_encode(single_data_transfer_t instr) {
    //TODO
    // cond = AL;
}

// word_t assemble_sdt(tokenized_instr_t *tokenized_instr, word_t *extra_exp,
//        int max_instr, int curr_instr, int extra_exp_size) {
void assemble_sdt(instr_list_t *il, symbol_table_t *st) {
    tokenized_instr_t *instr = il->curr->tokenized_instr;
    single_data_transfer_t sdt; 

    sdt.u = 1;
    sdt.rd = GET_REG_FROM_STR(instr->operands[1]);
    sdt.imm = 0;

    if (get_mnemonic(instr->operands[0])== LDR) {
        //loads from memory to register
        //requires L bit to be set
        sdt.l = 1;
    } else {
        //str, requires L bit to be clear
        sdt.l = 0;
    }

    if(instr->operands[2][0] == '=') {
        //in form <=expression>

        word_t expression = parse_numerical_expr(instr->operands[2]);
        //value of expression

        if (expression <= 0xFF) {
            //value of exp fits into argument of mov
            //compile instr as a mov instead of an ldr

            tokenized_instr_t *mov_tokens = malloc(sizeof(tokenized_instr_t));
            if (!mov_tokens) {
                perror("Unable to allocate memory for mov_tokens in assemble_sdt");
                exit(EXIT_FAILURE);
            }

            mov_tokens->operands = malloc(3 * sizeof(char));
            if (!mov_tokens) {
                perror("Unable to allocate memory for mov_tokens->operands in assemble_sdt");
                exit(EXIT_FAILURE);
            }

            mov_tokens->operands[0] = "mov";
            mov_tokens->operands[1] = instr->operands[1];
            mov_tokens->operands[2] = instr->operands[2];
            mov_tokens->operands[2][0] = '#';

            word_t mov_instr = assemble_dp(mov_tokens);
            free(mov_tokens->operands);
            free(mov_tokens);

            return mov_instr;

        } else {
            //put value of expression at end of assembled program
            sdt.p = 1;
            sdt.rn = 15; //register number of PC
            uint16_t address = ldr_add(il, expression);
            sdt.offset = address - il->curr->address - 8;
            /*
            sdt.imm = (((max_lines + extra_exp_size) - current_line) << 2) - 8;
            if (extra_exp_size >= sizeof(extra_exp)) {
                extra_exp_size++;
                extra_exp = realloc(extra_exp, extra_exp_size * sizeof(word_t));
                if (!extra_exp) {
                    perror("Unable to increase size of extra_exp in assemble_sdt");
                    exit(EXIT_FAILURE);
                }
            }
            extra_exp[extra_exp_size] = expression;
            */
        }

    } else {
        if (instr->operands[4][0] == ']') {
            //post-indexing addressing specification
            if(instr->operands[5][0] == '#') {
                //[Rn],<#expression>
                sdt.rn = GET_REG_FROM_STR(instr->operands[3]);
                sdt.offset = parse_numerical_expr(instr->operands[5]);
            } else {
                //TODO: [Rn],{+/-}Rm{,<shift>} (OPTIONAL)
            }

        } else {
            //pre-indexed address specification
            //[Rn]:
            sdt.p = 1;
            sdt.rn = GET_REG_FROM_STR(instr->operands[3]);
            if ([Rn, <#expression>]) {
                //[Rn, <#expression>]
                 word_t exp = parse_numerical_expr(instr->operands[4]);
                 if (exp >> 31) {
                     sdt.offset = (~exp) + 1;
                 } else {
                     sdt.offset = exp;
                 }
                 instr.flag_2 = (exp >> 31);
            } else {
                //TODO: [Rn,{+/-}Rm{,<shift>}] (OPTIONAL)
            }
        }
    }

    return sdt_encode(sdt);
}





