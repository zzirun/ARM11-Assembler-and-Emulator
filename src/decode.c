#include <stdlib.h>
#include "types.h"
#include "utils.h"

void decode(machine_state* ms) {
  decoded_inst result;
  uint32_t inst = ms -> instrFetched;

  // common to all instructions - top 4 bits : condition code
  result.cond = inst >> 28; 

  if (!inst) {
    result.type = HALT;
  } else if ((inst >> 26) & 0x1) {
    result.type = DATA_TRANS;
    result.imm = (inst >> 25) & 0x1;
    result.p = (inst >> 24) & 0x1;
    result.u = (inst >> 23) & 0x1;
    result.l = (inst >> 20) & 0x1;
    result.rn = (inst >> 16) & 0xF;
    // checks if Rn is PC
    // according to implementation, PC is at index 13 of regs
    if (result.rn == 15) { 
      result.rn -= 2; 
    }
    result.rd = (inst >> 12 ) & 0xF;
    result.operand_offset = inst & 0xFFF;
  } else if ((inst >> 27) & 0x1) {
    result.type = BRANCH;
    result.operand_offset = inst & 0xFFFFFF;
  } else if (((inst >> 4) & 0xF) == 0x9) { //0x9 in binary is 1001
    result.type = MULT;
    result.accum = (inst >> 21) & 0x1;
    result.set_cc = (inst >> 20) & 0x1;
    result.rd = (inst >> 16) & 0xF;
    result.rn = (inst >> 12) & 0xF;
    result.rs = (inst >> 8) & 0xF;
    result.rm = inst & 0xF;
  } else {
    result.type = DATA_PROC;
    result.imm = (inst >> 25) & 0x1;
    result.operation_code = (inst >> 21) & 0xF;
    result.set_cc = (inst >> 20) & 0x1;;
    result.rn = (inst >> 16) & 0xF;
    result.rd = (inst >> 12) & 0xF;
    result.operand_offset = inst & 0xFFF;
  }
  ms->instrToExecute = result;
}
