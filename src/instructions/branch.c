#include <stdint.h>
#include <stdlib.h>
#include "../utils.h"
#include "../types.h"
#include "datafunctions.h"

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
