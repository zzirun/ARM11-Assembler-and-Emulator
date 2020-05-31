#include <stdlib.h>
#include "types.h"
#include "utils.h"

void decode(machine_state* ms) {
  decoded_instr *decoded = &ms->instr_to_execute;
  word_t instr = ms -> instr_fetched;

  // common to all instructions - top 4 bits : condition code
  decoded->cond = instr >> (WORD_SIZE -4);

  if (!instr) {
    decoded->type = HALT;
  } else if ((instr >> 26) & GET_1_MASK) {
    decoded->type = DATA_TRANS;
    decoded->sdt.imm = (instr >> 25) & GET_1_MASK;
    decoded->sdt.p = (instr >> 24) & GET_1_MASK;
    decoded->sdt.u = (instr >> 23) & GET_1_MASK;
    decoded->sdt.l = (instr >> 20) & GET_1_MASK;
    decoded->sdt.rn = (instr >> 16) & GET_4_MASK;
    // checks if Rn is PC
    // according to implementation, PC is at index 13 of regs
    if (decoded->sdt.rn == 15) {
      decoded->sdt.rn -= 2;
    }
    decoded->sdt.rd = (instr >> 12 ) & GET_4_MASK;
    decoded->sdt.offset = instr & GET_12_MASK;
  } else if ((instr >> 27) & GET_1_MASK) {
    decoded->type = BRANCH;
    decoded->br.offset = instr & GET_24_MASK;
  } else if (((instr >> 4) & GET_4_MASK) == 0x9) { //0x9 in binary is 1001
    decoded->type = MULT;
    decoded->mul.accum = (instr >> 21) & GET_1_MASK;
    decoded->mul.set_cc = (instr >> 20) & GET_1_MASK;
    decoded->mul.rd = (instr >> 16) & GET_4_MASK;
    decoded->mul.rn = (instr >> 12) & GET_4_MASK;
    decoded->mul.rs = (instr >> 8) & GET_4_MASK;
    decoded->mul.rm = instr & GET_4_MASK;
  } else {
    decoded->type = DATA_PROC;
    decoded->dp.imm = (instr >> 25) & GET_1_MASK;
    decoded->dp.opcode = (instr >> 21) & GET_4_MASK;
    decoded->dp.set_cc = (instr >> 20) & GET_1_MASK;;
    decoded->dp.rn = (instr >> 16) & GET_4_MASK;
    decoded->dp.rd = (instr >> 12) & GET_4_MASK;
    decoded->dp.operand2 = instr & GET_12_MASK;
  }
}
