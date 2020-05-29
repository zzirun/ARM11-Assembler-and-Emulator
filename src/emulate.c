#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "utils.h"
#include "types.h"
#include "decode.h"

void binLoad(FILE* fp, uint8_t* array) {

  int read = 0; //Number of instructions read

  uint8_t* ptr = array; //Helper pointer to store instructions into array

  while (fread(ptr, 1, 1, fp) == 1) {
    read++;
    ptr++;
  }

/*
  //Debugging
  for (int i = 0; i < (read / 4) + 1; i++) {
    printf("%p : ", (void*) (array + 4 * i));
    printBits(buildInstruction(array + 4 * i));
  }
*/

}

void fetch(machine_state* ms) {
  ms->instrFetched = buildInstruction(&(ms->mem[ms->regs.PC]));
}

void update_processor_state(machine_state* ms) {
  if (ms->ps == EMPTY) {
    ms->ps = FETCHED;
  } else if (ms->ps == FETCHED) {
    ms->ps = DECODED;
  }
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

  fseek(fp, 0, SEEK_END); //Navigates to end of file to get size
  int no_of_inst = (ftell(fp) / 4) + 1; //+1 to add HALT inst at the end
  rewind(fp); // Returns to beginning of file


  binLoad(fp, ms->mem);
  fclose(fp);

  while (ms->regs.PC <= 4 * no_of_inst) {
    switch (ms->ps) {
      case (DECODED):
        execute(ms);
      case (FETCHED):
        decode(ms);
      default:
        fetch(ms);
        ms->regs.PC += 4;
        update_processor_state(ms);
    }
  }

	execute(ms);

  free(ms);

  return EXIT_SUCCESS;
}
