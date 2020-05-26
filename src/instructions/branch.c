#include <stdint.h>
#include <stdlib.h>
#include "instructions.h"
#include "../utils.h"
#include "../types.h"

void branch(decoded_inst* inst, registers* rs) {
  int32_t offset = inst -> branch_offset;
  offset <<= 2;
  if (offset < 0) {
    offset = abs(offset);
    rs -> PC -= offset;
  } else {
    rs -> PC += offset;
  }
}
