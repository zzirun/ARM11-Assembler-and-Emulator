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

//Builds 32 bit instructions according to little endian format
uint32_t buildInstruction(uint8_t* ptr) {
  uint32_t result = 0;
  for (int i = 0; i < 4; i++) {
    result += *(ptr+i) << (8*i);
  }
  return result;
}

//Builds 32 bit non zero value according to little endian format
uint32_t buildNonZeroValue(uint8_t* ptr) {
  uint32_t result = 0;
  for (int i = 0; i < 4; i++) {
    result += *(ptr+i) << (8 * (3-i));
  }
  return result;
}

//Shifter: returns shifted operand and updates carry
//param: shiftT(ype), op(erand), shift(amount), *carry
uint32_t shifter(shiftType shiftT, uint32_t op, uint8_t shift, bool *carry) {
  // carry always last discarded/rotated bit
  if (shiftT == LSL) {
    // logical shift left
    *carry = extractBits(op, 32 - shift, 32 - shift);
    return op << shift;
  }
  *carry = extractBits(op, shift - 1, shift - 1);
  switch (shiftT) {
    case LSR:
      // logical shift right
      return op >> shift;
    case ASR:
      // arithmetic shift right
      uint32_t mask = 0;
      if (extractBits(op, 31, 31)) {
        mask = (pow(2, shift) - 1) << (32 - shift);
      }
      return (op >> shift) | mask;
    case ROR:
      // rotate right
      uint32_t mask = extractBits(op, 0, shift - 1) << (32 - shift);
      return (op >> shift) | mask;
    default:
      fprintf(stderr, "Invalid Shift Instruction");
  }
}
