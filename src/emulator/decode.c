#include "emulate.h"
#include "emulate_instruction_utils.h"

void decode(machine_state* ms) {
  decoded_instr_t *decoded = &ms->instr_to_execute;
  word_t instr = ms -> instr_fetched;

  // common to all instructions - top 4 bits : condition code
  decoded->cond = instr >> (WORD_SIZE -4);

  if (!instr) {
    decoded->type = HALT;
  } else if ((instr >> 26) & GET_LS_1) {
    decoded->type = DATA_TRANS;
    decoded->sdt.imm = (instr >> 25) & GET_LS_1;
    decoded->sdt.p = (instr >> 24) & GET_LS_1;
    decoded->sdt.u = (instr >> 23) & GET_LS_1;
    decoded->sdt.l = (instr >> 20) & GET_LS_1;
    decoded->sdt.rn = (instr >> 16) & GET_LS_4;
    // checks if Rn is PC
    // according to implementation, PC is at index 13 of regs
    if (decoded->sdt.rn == 15) {
      decoded->sdt.rn -= 2;
    }
    decoded->sdt.rd = (instr >> 12 ) & GET_LS_4;
    decoded->sdt.offset = instr & GET_LS_12;
  } else if ((instr >> 27) & GET_LS_1) {
    decoded->type = BRANCH;
    decoded->br.offset = instr & GET_LS_24;
  } else if (((instr >> 4) & GET_LS_4) == 0x9) { //0x9 in binary is 1001
    decoded->type = MULT;
    decoded->mul.accum = (instr >> 21) & GET_LS_1;
    decoded->mul.set_cc = (instr >> 20) & GET_LS_1;
    decoded->mul.rd = (instr >> 16) & GET_LS_4;
    decoded->mul.rn = (instr >> 12) & GET_LS_4;
    decoded->mul.rs = (instr >> 8) & GET_LS_4;
    decoded->mul.rm = instr & GET_LS_4;
  } else {
    decoded->type = DATA_PROC;
    decoded->dp.imm = (instr >> 25) & GET_LS_1;
    decoded->dp.opcode = (instr >> 21) & GET_LS_4;
    decoded->dp.set_cc = (instr >> 20) & GET_LS_1;;
    decoded->dp.rn = (instr >> 16) & GET_LS_4;
    decoded->dp.rd = (instr >> 12) & GET_LS_4;
    decoded->dp.operand2 = instr & GET_LS_12;
  }
}
