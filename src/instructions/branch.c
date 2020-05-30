#include <stdint.h>
#include <stdlib.h>
#include "../utils.h"
#include "../types.h"
#include "datafunctions.h"

void branch(machine_state *ms) {
  int32_t offset = ms->instrToExecute.branch_offset;
  offset <<= 2;
  if ((offset & (1 << 25)) != 0) { //check if bit 25 is 1
    offset = ((63 << 26) | offset); //63 = pow(2, 7) - 1
  }
  ms->regs.PC += offset;
  ms->ps = EMPTY;
}
