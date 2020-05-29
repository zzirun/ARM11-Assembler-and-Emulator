#include <stdint.h>
#include <stdlib.h>
#include "instructions.h"
#include "../utils.h"
#include "../types.h"
#include "datafunctions.h"

//void branch(decoded_inst* inst, registers* rs) {
void branch(machine_state *ms) {
  int32_t offset = ms->instrToExecute.branch_offset;
  offset = shift_2_sign_extension(offset);
  ms->regs.PC += offset;
  ms->ps = EMPTY;
}
