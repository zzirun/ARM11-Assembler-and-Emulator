#include <stdio.h>
#include <stdlib.h>
#include "datafunctions.h"
#include "../types.h"
#include "../utils.h"

//Output at termination
void output(machine_state* ms) {
  printf("Registers:\n");
  for (int i = 0; i < 13; i++) {
    printf("$%-3d: %10d (0x%08x)\n", i, *(ms->regs.gpr+i), *(ms->regs.gpr+i));
  }
  printf("PC  : %10d (0x%08x)\n", ms->regs.PC, ms->regs.PC);
  printf("CPSR: %10d (0x%08x)\n", ms->regs.CPSR, ms->regs.CPSR);
  printf("Non-zero memory:\n");
  for (int i = 0; i < 65536; i += 4) {
    uint32_t x = buildNonZeroValue(ms->mem+i);
    if (x > 0) {
      printf("0x%08x: 0x%08x\n", i, x);
    }
  }
}

void halt(machine_state* ms) {
  output(ms);
  free(ms);
  exit(EXIT_SUCCESS);
}
