#include <stdint.h>
#include <stdlib.h>
#include "instructions.h"
#include "../utils.h"
#include "../types.h"
#include "datafunctions.h"

//void branch(decoded_inst* inst, registers* rs) {
void branch(machine_state *ms) {
  int32_t offset = ms->instrToExecute.branch_offset;
  offset <<= 2;
  if (offset < 0) {
    offset = abs(offset);
    ms->regs.PC -= offset;
  } else {
    ms->regs.PC += offset;
  }
  ms->ps = EMPTY;
}
