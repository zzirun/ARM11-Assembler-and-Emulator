#include <stdlib.h>
#include "types.h"
#include "instructions/instructions.h"
#include "utils.h"
#include "decode.h"

//To call decoder
void decode(machine_state* ms) {
  decoded_inst result;
  uint32_t inst = ms -> instrFetched;
  result.cond = extractBits(inst, 28, 31); //Common to all instructions
  if (extractBits(inst, 0, 31) == 0) {
    result.type = HALT;
  } else if (extractBits(inst, 26, 26) == 1) {
    result.type = DATA_TRANS;
    result.I = extractBits(inst, 25, 25);
    result.P = extractBits(inst, 24, 24);
    result.U = extractBits(inst, 23, 23);
    result.L = extractBits(inst, 20, 20);
    result.Rn = extractBits(inst, 16, 19);
    result.Rd = extractBits(inst, 12, 15);
    result.operand_offset = extractBits(inst, 0, 11);
  } else if (extractBits(inst, 27, 27) == 1) {
    result.type = BRANCH;
    result.branch_offset = extractBits(inst, 0, 23);
  } else if (extractBits(inst, 4, 7) == 9) { //9 in binary is 1001
    result.type = MULT;
    result.A = extractBits(inst, 21, 21);
    result.S = extractBits(inst, 20, 20);
    result.Rd = extractBits(inst, 16, 19);
    result.Rn = extractBits(inst, 12, 15);
    result.Rs = extractBits(inst, 8, 11);
    result.Rm = extractBits(inst, 0, 3);
  } else {
    result.type = DATA_PROC;
    result.I = extractBits(inst, 25, 25);
    result.operation_code = extractBits(inst, 21, 24);
    result.S = extractBits(inst, 20, 20);
    result.Rn = extractBits(inst, 16, 19);
    result.Rd = extractBits(inst, 12, 15);
    result.operand_offset = extractBits(inst, 0, 11);
  }
  ms->instrToExecute = result;
}
