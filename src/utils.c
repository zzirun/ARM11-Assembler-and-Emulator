#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "utils.h"

//start >= 0 && end <= 31
uint32_t extractBits(uint32_t target, int start, int end) {
  int size = end - start + 1;
  uint32_t mask = pow(2, size) - 1;
  mask <<= start;
  target &= mask;
  target >>= start;
  return target;
}

//For debugging
void printBits(uint32_t x) {
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

uint32_t buildInstruction(uint8_t* ptr) {
  uint32_t result = 0;
  for (int i = 0; i < 4; i++) {
    result += *(ptr+i) << (8*i);
  }
  return result;
}

uint32_t buildNonZeroValue(uint8_t* ptr) {
  uint32_t result = 0;
  for (int i = 0; i < 4; i++) {
    result += *(ptr+i) << (8 * (3-i));
  }
  return result;
}
