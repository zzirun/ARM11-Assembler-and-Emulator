#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "utils.h"
#include "types.h"

void print_bits(word_t x) {
  word_t mask = 1 << 31;
  for (int i = 0; i < 32; i++) {
    if ((x & mask) == 0 ) {
      printf("0");
    } else {
      printf("1");
    }
    x <<= 1;
  }
  printf("\n");
}

void bin_load(FILE *fp, byte_t *array) {
    int read = 0; //Number of instructions read
    byte_t *ptr = array; //Helper pointer to store instructions into array
    while (fread(ptr, 1, 1, fp) == 1) {
        read++;
        ptr++;
    }
}

word_t load_word(word_t address, machine_state *ms) {
    if (address > ADDRESS_COUNT - 4) {
        printf("Error: Out of bounds memory access at address 0x%08x\n", address);
        return 0;
    } else {
      word_t word = 0;
      for (size_t i = 0; i < 4; i++) {
          word |= ms->mem[address + i] << (i * 8);
      }
      return word;
    }
}

void store_word(word_t address, machine_state *ms, word_t word) {
    if (address > ADDRESS_COUNT - 4) {
        printf("Error: Out of bounds memory access at address 0x%08x\n", address);
    } else {
      for (size_t i = 0; i < 4; i++) {
          ms->mem[address + i] = word & 0xFF;
          word >>= 8;
      }
    }
}

word_t shifter(shift_type shift_t, word_t op, byte_t shift, bool *carry) {
  if (shift == 0) {
    return op;
  }
  // carry always last discarded/rotated bit
  if (shift_t == LSL) {
    // logical shift left
    *carry = ((op >> (32 - shift)) & 0x1); //extractBits(op, 32 - shift, 32 - shift)
    return op << shift;
  }
  *carry = ((op >> (shift - 1)) & 0x1); //extractBits(op, shift - 1, shift - 1)
  word_t mask = 0;
  switch (shift_t) {
    case LSR:
      // logical shift right
      return op >> shift;
    case ASR:
      // arithmetic shift right
      if ((op >> 31) & 0x1) { //extractBits(op, 31, 31)
        mask = ((1 << shift) - 1) << (32 - shift); //(int) (pow(2, shift) - 1) << (32 - shift)
      }
      return (op >> shift) | mask;
    case ROR:
      // rotate right
      mask = (op & ((1 << (shift - 1)) - 1)) << (32 - shift); //extractBits(op, 0, shift - 1) << (32 - shift)
      return (op >> shift) | mask;
    default:
      fprintf(stderr, "Invalid Shift Instruction");
      exit(EXIT_FAILURE);
  }
}

//Helper for output function
word_t build_nonzero_value(byte_t* ptr) {
  word_t result = 0;
  for (int i = 0; i < 4; i++) {
    result += *(ptr+i) << (8 * (3-i));
  }
  return result;
}

void output(machine_state* ms) {
  printf("Registers:\n");
  for (int i = 0; i < 13; i++) {
    printf("$%-3d: %10d (0x%08x)\n", i, *(ms->regs.gpr+i), *(ms->regs.gpr+i));
  }
  printf("PC  : %10d (0x%08x)\n", ms->regs.pc, ms->regs.pc);
  printf("CPSR: %10d (0x%08x)\n", ms->regs.cpsr, ms->regs.cpsr);
  printf("Non-zero memory:\n");
  for (int i = 0; i < 65536; i += 4) {
    word_t x = build_nonzero_value(ms->mem+i);
    if (x > 0) {
      printf("0x%08x: 0x%08x\n", i, x);
    }
  }
}

void terminate(machine_state* ms){
    free(ms);
    exit(EXIT_FAILURE);
}
