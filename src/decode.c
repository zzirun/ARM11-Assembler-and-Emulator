#include <stdlib.h>
#include "types.h"
#include "utils.h"

void decode(machine_state* ms) {
  decoded_instr *decoded = &ms->instr_to_execute;
  word_t instr = ms -> instr_fetched;

  // common to all instrructions - top 4 bits : condition code
  decoded->cond = instr >> 28;

  if (!instr) {
    decoded->type = HALT;
  } else if ((instr >> 26) & 0x1) {
    decoded->type = DATA_TRANS;
    decoded->sdt.imm = (instr >> 25) & 0x1;
    decoded->sdt.p = (instr >> 24) & 0x1;
    decoded->sdt.u = (instr >> 23) & 0x1;
    decoded->sdt.l = (instr >> 20) & 0x1;
    decoded->sdt.rn = (instr >> 16) & 0xF;
    // checks if Rn is PC
    // according to implementation, PC is at index 13 of regs
    if (decoded->sdt.rn == 15) {
      decoded->sdt.rn -= 2;
    }
    decoded->sdt.rd = (instr >> 12 ) & 0xF;
    decoded->sdt.offset = instr & 0xFFF;
  } else if ((instr >> 27) & 0x1) {
    decoded->type = BRANCH;
    decoded->br.offset = instr & 0xFFFFFF;
  } else if (((instr >> 4) & 0xF) == 0x9) { //0x9 in binary is 1001
    decoded->type = MULT;
    decoded->mul.accum = (instr >> 21) & 0x1;
    decoded->mul.set_cc = (instr >> 20) & 0x1;
    decoded->mul.rd = (instr >> 16) & 0xF;
    decoded->mul.rn = (instr >> 12) & 0xF;
    decoded->mul.rs = (instr >> 8) & 0xF;
    decoded->mul.rm = instr & 0xF;
  } else {
    decoded->type = DATA_PROC;
    decoded->dp.imm = (instr >> 25) & 0x1;
    decoded->dp.opcode = (instr >> 21) & 0xF;
    decoded->dp.set_cc = (instr >> 20) & 0x1;;
    decoded->dp.rn = (instr >> 16) & 0xF;
    decoded->dp.rd = (instr >> 12) & 0xF;
    decoded->dp.operand2 = instr & 0xFFF;
  }
}
