#include <stdlib.h>
#include "types.h"
#include "instructions/instructions.h"
#include "utils.h"

void decode(machine_state* ms) {
  decoded_inst result;
  uint32_t inst = ms -> instrFetched;
  result.cond = inst >> 28; //Common to all instructions
  if (!inst) {
    result.type = HALT;
  } else if ((inst >> 26) & 0x1) {
    result.type = DATA_TRANS;
    result.I = (inst >> 25) & 0x1;
    result.P = (inst >> 24) & 0x1;
    result.U = (inst >> 23) & 0x1;
    result.L = (inst >> 20) & 0x1;
    result.Rn = (inst >> 16) & 0xF;
    if (result.Rn == 15) { //Checks if Rn is PC
      result.Rn -= 2; //According to implementation, PC is at index 13 of regs
    }
    result.Rd = (inst >> 12 ) & 0xF;
    result.operand_offset = inst & 0xFFF;
  } else if ((inst >> 27) & 0x1) {
    result.type = BRANCH;
    result.branch_offset = inst & 0xFFFFFF;
  } else if (((inst >> 4) & 0xF) == 0x9) { //0x9 in binary is 1001
    result.type = MULT;
    result.A = (inst >> 21) & 0x1;
    result.S = (inst >> 20) & 0x1;
    result.Rd = (inst >> 16) & 0xF;
    result.Rn = (inst >> 12) & 0xF;
    result.Rs = (inst >> 8) & 0xF;
    result.Rm = inst & 0xF;
  } else {
    result.type = DATA_PROC;
    result.I = (inst >> 25) & 0x1;
    result.operation_code = (inst >> 21) & 0xF;
    result.S = (inst >> 20) & 0x1;;
    result.Rn = (inst >> 16) & 0xF;
    result.Rd = (inst >> 12) & 0xF;
    result.operand_offset = inst & 0xFFF;
  }
  ms->instrToExecute = result;
}
