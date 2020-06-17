#include "emulate.h"

void bin_load(char *f, machine_state *ms) {
  // Opens file
  FILE *fp;
  if (!(fp = fopen(f, "rb"))) {
      perror("Cannot Open File");
      terminate(ms);
  }
  byte_t *ptr = ms->mem; //Helper pointer to store instructions into array
  while (fread(ptr, 1, 1, fp) == 1) {
      ptr++;
  }
  fclose(fp);
}

void output(machine_state *ms) {
  printf("Registers:\n");
  for (int i = 0; i < 13; i++) {
    printf("$%-3d: %10d (0x%08x)\n", i, *(ms->regs.gpr + i), *(ms->regs.gpr + i));
  }
  printf("PC  : %10d (0x%08x)\n", ms->regs.pc, ms->regs.pc);
  printf("CPSR: %10d (0x%08x)\n", ms->regs.cpsr, ms->regs.cpsr);
  printf("Non-zero memory:\n");
  for (int i = 0; i < ADDRESS_COUNT; i += 4) {
    word_t x = build_nonzero_value(ms->mem + i);
    if (x > 0) {
      printf("0x%08x: 0x%08x\n", i, x);
    }
  }
}