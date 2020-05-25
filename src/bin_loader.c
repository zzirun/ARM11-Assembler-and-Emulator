#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

//debugging
void printBits(uint32_t x){
 uint32_t mask = 1 << 31;
for(int i=0; i<32;i++){
 if((x & mask) == 0 ){
  printf("0");
 }
 else{
  printf("1");
 }
 x <<= 1;
}
printf("\n");
}

uint32_t* binLoad(char* filename) {
  FILE* fp = fopen(filename, "r");
  fseek(fp, 0, SEEK_END);
  long size = ftell(fp); //size of file
  rewind(fp);

  uint32_t* ptr = (uint32_t*) malloc(size); //Base memory address

  int read = 0; //Number of instructions read

  uint32_t* pp = ptr; //Helper pointer to store insturctions in heap

  while (!feof(fp)) {
    int i  = fread(pp, 4, 1, fp);
    read += i;
    pp++;
  }

  //Debugging
  for (int i = 0; i < read; i++) {
    printf("%p : ", (void*) (ptr + i));
    printBits(*(ptr + i));
  }

  return ptr;
}

int main(int argc, char** argv) {
  binLoad(argv[1]); //argv[1] to skip space character
  return 0;
}
