#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>

#define PC   *(rs.rBase+13) //Value of PC register
#define CPSR *(rs.rBase+14) //Value of CPSR register

//dp is data processing, sdt is single data transfer
typedef enum instruction_set {dp, mult, sdt, branch} instruction_set;

char* enumStrings[4] = {"dp", "mult", "sdt", "branch"};

typedef struct registers {
  int rBase[15];
} reg;

//start >= 0 && end <= 31
uint32_t extractBits(uint32_t target, int start, int end) {
  int size = end - start + 1;
  uint32_t mask = pow(2, size) - 1;
  mask <<= start;
  target &= mask;
  target >>= start;
  return target;
}

/*
  Refer to spec
*/
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

//Debug bin_loader
void printBits(uint32_t x){
  uint32_t mask = 1 << 31;
  for(int i=0; i<32;i++) {
    if((x & mask) == 0 ) {
      printf("0");
    } else {
      printf("1");
    }
    x <<= 1;
  }
  printf("\n");
}

void binLoad(FILE* fp, uint32_t* array) {

  int read = 0; //Number of instructions read

  uint32_t* ptr = array; //Helper pointer to store instructions into array

  while (!feof(fp)) {
    int i  = fread(ptr, 4, 1, fp);
    read += i;
    ptr++;
  }

  //Debugging
  for (int i = 0; i < read; i++) {
    printf("%p : (%s) ", (void*) (array + i), enumStrings[decode(*(array+i))]);
    printBits(*(array + i));
  }

}

//Output at termination
void printReg(reg rs) {
  printf("Registers:\n");
  for (int i = 0; i < 13; i++) {
    printf("$%-3d:          %d (0x%08x)\n", i, *(rs.rBase+i), *(rs.rBase+i));
  }
  printf("PC  :          %d (0x%08x)\n", PC, PC);
  printf("CPSR:          %d (0x%08x)\n", CPSR, CPSR);
}

int main(int argc, char **argv) {
  FILE* fp = fopen(argv[1], "r"); //Opens file, argv[1] to skip space character
  fseek(fp, 0, SEEK_END); //Navigates to end of file to get size
  long filesize = ftell(fp); //size of file
  rewind(fp); // Returns to beginning of file

  int arraysize = filesize / 4; //Size of instructionsArray

  uint32_t instructionsArray[arraysize];

  binLoad(fp, instructionsArray);

  reg* rs = (reg*) calloc(15, 4);
  printReg(*rs);

  return EXIT_SUCCESS;
}
