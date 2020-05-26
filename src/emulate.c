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

  while (!feof(fp)) {
    int i  = fread(ptr, 1, 1, fp);
    read += i;
    ptr++;
  }

  //Debugging
  for (int i = 0; i < read / 4; i++) {
    printf("%p : ", (void*) (array + 4 * i));
    printBits(buildInstruction(array + 4 * i));
  }

}

//Output at termination
void output(registers rs, uint8_t* ptr) {
  printf("Registers:\n");
  for (int i = 0; i < 13; i++) {
    printf("$%-3d:          %d (0x%08x)\n", i, *(rs.gpr+i), *(rs.gpr+i));
  }
  printf("PC  :          %d (0x%08x)\n", rs.PC, rs.PC);
  printf("CPSR:          %d (0x%08x)\n", rs.CPSR, rs.CPSR);
  printf("Non zero memory : \n");
  //TODO : Add printf for non zero memory
}

int main(int argc, char **argv) {
  //Memory of ARM emulator has 64KB, initialised to all zero
  uint8_t memory[65536] = {0};

  FILE* fp = fopen(argv[1], "r"); //Opens file, argv[1] to skip space character
  /*
  //Unused now but may be useful later?
  fseek(fp, 0, SEEK_END); //Navigates to end of file to get size
  long filesize = ftell(fp); //size of file
  int arraysize = filesize / 4; //Size of instructionsArray
  rewind(fp); // Returns to beginning of file
  */
  binLoad(fp, memory);
  fclose(fp);

  registers* rs = (registers*) calloc(15, 4);
  output(*rs, memory);

  machine_state* ms = (machine_state*) calloc(28, 1);
  decode(buildInstruction(&memory[4]), ms);

  return EXIT_SUCCESS;
}
