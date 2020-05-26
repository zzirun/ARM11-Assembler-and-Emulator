#include<stdio.h>
#include<stdint.h>

int main(void) {
  uint8_t memory[65536] = {0};
  uint8_t* ptr = memory;
  printf("%p : %d\n", (void*) ptr, *ptr);
  printf("%p : %d\n", (void*) &(memory[15535]), memory[15535]);
  printf("%p : %d\n", (void*) &(memory[25535]), memory[25535]);
  printf("%p : %d\n", (void*) &(memory[35535]), memory[35535]);
  printf("%p : %d\n", (void*) &(memory[45535]), memory[45535]);
  printf("%p : %d\n", (void*) &(memory[55535]), memory[55535]);
  printf("%p : %d\n", (void*) &(memory[65535]), memory[65535]);
}
