#include <stdint.h>
#include <stdlib.h>
#include "../utils.h"
#include "../types.h"
#include "datafunctions.h"

void branch(machine_state *ms) {
  int32_t offset = ms->instr_to_execute.br.offset;
  offset <<= 2;
  // sign extension
  if ((offset >> 25) & 0x1) { //check if bit 25 is 1
    offset |= (63 << 26); //63 = pow(2, 7) - 1 (top 6)
  }
  // set PC to new address
  ms->regs.pc += offset;
  // reset pipeline
  ms->ps = EMPTY;
}
