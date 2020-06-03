//
// Created by Zhai Zirun on 3/6/20.
//

#include "data_functions.h"

#include <stdint.h>
#include <stdlib.h>

#include "../utils.h"
#include "../types.h"
#include "operand_extraction.h"

void branch(machine_state *ms) {
    int32_t offset = ms->instr_to_execute.br.offset;
    offset <<= 2;
    // sign extension
    if ((offset >> 25) & GET_1_MASK) { // check if bit 25 is 1
        offset |= NEG_SIGN_EXT_MASK;
    }
    // set PC to new address
    ms->regs.pc += offset;
    // reset pipeline
    ms->ps = EMPTY;
}

void data_processing(machine_state *ms) {
    data_processing_t *instr = &ms->instr_to_execute.dp;
    word_t op1 = ms->regs.gpr[instr->rn];
    word_t op2;
    word_t result;

    // logical op (AND, EOR, ORR, TEQ, TST, MOV) : shifter carry
    // arithmetic op (ADD, SUB, RSB, CMP) : ALU carry
    bool carry = 0;

    // Assign second operand
    // Set all carry to shifter carry
    if (instr->imm) {
        // rotated 8-bit Imm
        op2 = imm_extract(instr->operand2, &carry);
    } else {
        //shifted reg
        op2 = reg_extract(instr->operand2, ms, &carry);
    }

    // Arithmetic/logic operation based on opcode
    // Set carry for arithmetic op (ADD, SUB, CMP, RSB) to ALU carry
    switch (instr->opcode) {
        case AND:
        case TST:
            result = op1 & op2;
            break;
        case EOR:
        case TEQ:
            result = op1 ^ op2;
            break;
        case SUB:
        case CMP:
            result = op1 - op2;
            // set carry if no borrow
            carry = op1 < op2 ? 0 : 1;
            break;
        case RSB:
            result = op2 - op1;
            // set carry if no borrow
            carry = op2 < op1 ? 0 : 1;
            break;
        case ADD:
            result = op1 + op2;
            // set carry if unsigned overflow
            carry = result < op1 ? 1 : 0;
            break;
        case ORR:
            result = op1 | op2;
            break;
        case MOV:
            result = op2;
            break;
        default:
            fprintf(stderr, "Invalid Data Processing Opcode");
            terminate(ms);
    }

    // Write to destination for specific operations
    switch (instr->opcode) {
        case TST:
        case TEQ:
        case CMP:
            // don't write
            break;
        default:
            // destReg = result
            ms->regs.gpr[instr->rd] = result;
    }

    /* Update CPSR flags if S bit set
     * CPSR - bit 31 - 28 : NZCV
       * V unaffected
       * C set to carry (ALU/Shifter):
       * Z set only if result all 0s
       * N set to logical bit 31 of result
     */
    if (instr->set_cc) {
        word_t flags_new = (C * carry) | (Z * (result == 0)) | (N * (result >> 31));
        // clear top 3 bits and set to new flags
        ms->regs.cpsr = (ms->regs.cpsr & GET_29_MASK) | (flags_new << 28);
    }
}


void multiply(machine_state* ms){
    multiply_t *instr = &ms->instr_to_execute.mul;
    word_t result;

    // Multiplying Rm and Rs
    result = ms->regs.gpr[instr->rm] * ms->regs.gpr[instr->rs];

    // If A bit is set then accumulate with Rn
    if(instr->accum){
        result += ms->regs.gpr[instr->rn];
    }

    // Write to destination register
    ms->regs.gpr[instr->rd] = result;

    //Update flags if S bit is set
    if(instr->set_cc){
        // extract the top 4 bits of the cpsr
        word_t new_cpsr = (ms->regs.cpsr) >> (WORD_SIZE - 4);
        //set Z using mask
        if(result == 0){
            new_cpsr |= Z ;
        }
        //set N flag to 31 bit of result using mask
        if(result >> 31){
            new_cpsr |= N;
        }

        //Update cpsr to newcpsr
        ms->regs.cpsr = new_cpsr << (WORD_SIZE - 4);

    }
}


void single_data_transfer(machine_state *ms) {
    single_data_transfer_t *instr = &ms->instr_to_execute.sdt;
    word_t address;
    word_t offset;

    if (instr->imm) {
        // immediate flag is set, offset interpreted as
        // shifted register
        bool carry;
        offset = reg_extract(instr->offset, ms, &carry);
    } else {
        // immediate flag not set, offset interpreted as
        // an unsigned 12 bit immediate offset
        offset = instr->offset;
    }

    if (!instr->u) {
        // U flag not set, offset is subtracted from base register
        offset = (~offset) + 1;
    }

    if (instr->p) {
        // P flag is set, offset is added/subtracted
        // to the base register before transferring the data
        address = ms->regs.gpr[instr->rn] + offset;
    } else {
        // P flag is not set, offset is added/subtracted
        // to the base register after transferring
        address = ms->regs.gpr[instr->rn];
        ms->regs.gpr[instr->rn] = address + offset;
    }

    if (instr->l) {
        // L bit is set, word is loaded from memory
        ms->regs.gpr[instr->rd] = load_word(address, ms);
    } else {
        // L bit is not set, word is stored into memory
        store_word(address, ms, ms->regs.gpr[instr->rd]);
    }
}
