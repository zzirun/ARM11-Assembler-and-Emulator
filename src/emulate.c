#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "utils.h"
#include "types.h"
#include "decode.h"

/*
  Refer to spec
instruction_set decode(uint32_t i) {
  if (extractBits(i, 26, 26) == 1) {
    return sdt;
  } else if (extractBits(i, 27, 27) == 1) {
    return branch;
  } else if (extractBits(i, 22, 27) == 0 && extractBits(i, 4, 7) == 9) {
    return mult;
  } else {
    return dp;
  }
}
*/

void binLoad(FILE* fp, uint8_t* array) {

  int read = 0; //Number of instructions read

  uint8_t* ptr = array; //Helper pointer to store instructions into array

  while (fread(ptr, 1, 1, fp) == 1) {
    read++;
    ptr++;
  }

  //Debugging
  for (int i = 0; i < read / 4; i++) {
    printf("%p : ", (void*) (array + 4 * i));
    printBits(buildInstruction(array + 4 * i));
  }

}

//Output at termination
void output(machine_state ms) {
  printf("Registers:\n");
  for (int i = 0; i < 13; i++) {
    printf("$%-3d:          %d (0x%08x)\n", i, *(ms.regs.gpr+i), *(ms.regs.gpr+i));
  }
  printf("PC  :          %d (0x%08x)\n", ms.regs.PC, ms.regs.PC);
  printf("CPSR:          %d (0x%08x)\n", ms.regs.CPSR, ms.regs.CPSR);
  printf("Non zero memory : \n");
  //TODO : Add printf for non zero memory
}

int main(int argc, char **argv) {
  /*
  Machine state when turned on is zero
  15 registers initialised to zero
  Memory of ARM emulator has 64KB, initialised to zero
  */
  machine_state* ms = (machine_state*) calloc(sizeof(machine_state), 1);
  // check for successful memory allocation
  if (!ms) {
    fprintf(stderr, "Unsuccessful Memory Allocation");
    return EXIT_FAILURE;
  }

  //Opens file, argv[1] to skip space character
  FILE* fp;
  if (!(fp = fopen(argv[1], "rb"))) {
    perror("Cannot Open File");
    terminate(ms);
  }; 
  /*
    //Unused now but may be useful later?
  fseek(fp, 0, SEEK_END); //Navigates to end of file to get size
  long filesize = ftell(fp); //size of file
  int arraysize = filesize / 4; //Size of instructionsArray
  rewind(fp); // Returns to beginning of file
  */
  binLoad(fp, ms->mem);
  fclose(fp);

  output(*ms);

  free(ms);

  return EXIT_SUCCESS;
}
