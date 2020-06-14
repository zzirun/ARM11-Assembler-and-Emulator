//
// Created by Zhai Zirun on 11/6/20.
//

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "../types.h"
#include "assemble.h"
#include "assemble_utils.h"


void assemble_sdt(program_t *prog, symbol_table_t *st) {
  instr_str_t *instr = prog->curr->instr_str;
    decoded_instr_t dec;
    dec.type = DATA_TRANS;
    dec.cond = AL;
    single_data_transfer_t *sdt = &dec.sdt;

    sdt->u = 1;
    sdt->rd = GET_REG_FROM_STR(instr->operands[0]);
    sdt->imm = 0;
    sdt->p = 0;
    //for optional shift portion
    reg_add_t rm = -1;
    reg_add_t rs = -1;
    byte_t shift_amount = -1;
    shift_type shift_type;

    if (instr->mnemonic == LDR) {
        //loads from memory to register
        //requires L bit to be set
        sdt->l = 1;
    } else {
        //str, requires L bit to be clear
        sdt->l = 0;
    }

    if(instr->operands[1][0] == '=') {
        //in form <=expression>

        word_t expression = parse_numerical_expr(instr->operands[1]);
        //value of expression

        if (expression <= 0xFF) {
            //value of exp fits into argument of mov
            //compile instr as a mov instead of an ldr
            instr->mnemonic = MOV;
            instr->operands[1][0] = '#';
            /*
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
            */
            assemble_dp(prog, st);
            return;
            // il->curr->binary_instr = mov_instr;

        } else {
            //put value of expression at end of assembled program
            sdt->p = 1;
            sdt->rn = 15; //register number of PC
            uint16_t address = add_data(prog, expression);
            sdt->offset = address - prog->curr->address - 8;
            /*
            sdt->imm = (((max_lines + extra_exp_size) - current_line) << 2) - 8;
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
      // pre or post indexed
      char *op = trim(strtok(instr->operands[1], "[]"));
      char *rem = strtok(NULL, "");
      if (rem) {
        // post indexing
        sdt->p = 0;
        sdt->rn = GET_REG_FROM_STR(op);
        //get_op_from_str(op, &dec);
        sdt->offset = parse_numerical_expr(instr->operands[5]);
      }
      else {
        // pre indexing
        sdt->p = 1;
        op = strtok(op, " ,");
        sdt->rn = GET_REG_FROM_STR(op);
        sdt->offset = 0;
        //op = trim(strtok(NULL, ""));
        //get_op_from_str(op, &dec);
        if ('#' == instr->operands[1][0]) {
            //[Rn, <#expression>]
            word_t exp = parse_numerical_expr(instr->operands[1]);
            if (exp >> 31) {
                sdt->offset = (~exp) + 1;
            } else {
                sdt->offset = exp;
            }
                sdt->u = (exp >> 31);
        }

      }
    }

    /*
    else {
        if (instr->operands[3][0] == ']') {
            //post-indexing addressing specification
            if(instr->operands[4][0] == '#') {
                //[Rn],<#expression>
                sdt->rn = GET_REG_FROM_STR(instr->operands[3]);
                sdt->offset = parse_numerical_expr(instr->operands[5]);
            } else {
                //[Rn],{+/-}Rm{,<shift>} (OPTIONAL)
                sdt->rn = string_to_reg_address(instr->operands[3]);
                sdt->imm = 1;
                rm = GET_REG_FROM_STR(instr->operands[5]);
                if (instr->operands[4][0] == '-') {
                    sdt->u = 0;
                }
                if (no_of_operands > 6) {
                    //shift
                    char *shift_str = malloc(sizeof(char) * sizeof(instr->operands[6]));
                    if (!shift_str) {
                        perror("Unable to allocate memory for shift_str in assemble_sdt");
                        exit(EXIT_FAILURE);
                    }

                    shift_str = &instr->operands[5];
                    shift_type = string_to_shift(shift_str[0]);
                    if ('#' == shift_str[1][0]) {
                        // <#expression>
                        shift_amount = parse_numerical_expr(&shift_str[1][1]);
                    } else if ('r' == shift_str[1][0]) {
                        // register
                        rs = GET_REG_FROM_STR(tokens->array[1]);
                    } else {
                        fprintf(stderr, "Shift not number or register");
                        exit(EXIT_FAILURE);
                    }

                    free(shift_str);
                }
            }

        } else {
            //pre-indexed address specification
            //[Rn]:
            sdt->p = 1;
            sdt->rn = GET_REG_FROM_STR(instr->operands[2]);
            if ('#' == instr->operands[3][0]) {
                //[Rn, <#expression>]
                word_t exp = parse_numerical_expr(instr->operands[3]);
                if (exp >> 31) {
                    sdt->offset = (~exp) + 1;
                } else {
                    sdt->offset = exp;
                }
                instr.u = (exp >> 31);
            } else {
                //[Rn,{+/-}Rm{,<shift>}] (OPTIONAL)
                sdt->imm = 1;
                sdt->rm = GET_REG_FROM_STR(instr->operands[3]);

                if ('-' == instr->operands[3][0]) {
                    sdt->u = 0;
                }

                if (no_of_operands > 6) {
                    // shift
                    char *shift_str = malloc(sizeof(char) * sizeof(instr->operands[4]));
                    if (!shift_str) {
                        perror("Unable to allocate memory for shift_str in assemble_sdt");
                        exit(EXIT_FAILURE);
                    }

                    shift_str = &instr->operands[4];
                    shift_type = string_to_shift(shift_str[0]);
                    if ('#' == shift_str[1][0]) {
                        // <#expression>
                        shift_amount = parse_numerical_expr(&shift_str[1][1]);
                    } else if ('r' == shift_str[1][0]) {
                        // register
                        rs = GET_REG_FROM_STR(tokens->array[1]);
                    } else {
                        fprintf(stderr, "Shift not number or register");
                        exit(EXIT_FAILURE);
                    }

                    free(shift_str);
                }
                
            }
        } */
    
  word_t bin = ((word_t) AL) << 28;
  bin |= 1 << 26;
  bin |= ((word_t) sdt->imm) << 25;
  bin |= ((word_t) sdt->p) << 24;
  bin |= ((word_t) sdt->u) << 23;
  bin |= ((word_t) sdt->l) << 20;
  bin |= ((word_t) sdt->rn) << 16;
  bin |= ((word_t) sdt->rd) << 12;
  bin |= ((word_t) sdt->offset) & 0xFFF;
    // if (rm != -1) {
    //     bin |= rm;
    // }
    // bin |= ((word_t) shift_type) << 5;
    // if (rs != -1) {
    //     // Shift by 1 register
    //     bin |= 1L << 4;
    //     bin |= ((word_t) rs) << 8;
    // } else {
    //     bin |= ((0x1F) & shift_amount) << 7;
    // }
    // } else {
    //     bin |= ((word_t) sdt->imm) & 0xFFF;
    // }

    //bin |= (word_t) 0xE) <<cd  28; //COND is AL

    free_instr_str(instr);
    prog->curr->binary = bin;
}





