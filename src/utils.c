#include "utils.h"
#include "emulate.h"

void terminate(machine_state* ms){
    free(ms);
    exit(EXIT_FAILURE);
}

word_t build_nonzero_value(byte_t *ptr) {
  word_t result = 0;
  for (int i = 0; i < 4; i++) {
    result += *(ptr + i) << (BYTE_SIZE * (3 - i));
  }
  return result;
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

word_t load_word(word_t address, machine_state *ms) {
    if (address > ADDRESS_COUNT - 4) {
        printf("Error: Out of bounds memory access at address 0x%08x\n", address);
        return 0;
    } else {
      word_t word = 0;
      for (size_t i = 0; i < 4; i++) {
          word |= ms->mem[address + i] << (i * BYTE_SIZE);
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
    *carry = ((op >> (32 - shift)) & 0x1); 
    return op << shift;
  }
  *carry = ((op >> (shift - 1)) & 0x1); 
  word_t mask = 0;
  switch (shift_t) {
    case LSR:
      // logical shift right
      return op >> shift;
    case ASR:
      // arithmetic shift right
      if ((op >> 31) & 0x1) { 
        mask = ((1 << shift) - 1) << (32 - shift); 
      }
      return (op >> shift) | mask;
    case ROR:
      // rotate right
      mask = (op & ((1 << (shift - 1)) - 1)) << (32 - shift); 
      return (op >> shift) | mask;
    default:
      fprintf(stderr, "Invalid Shift Instruction");
      exit(EXIT_FAILURE);
  }
}

/* Returns operand2 for DP, offset for SDT */

/* As a rotated 8-bit immediate constant
 * *splitting :
 *  * bit 7 - 0 : unsigned immediate value (byte)
 *  * bit 11 -8 : half of rotation amount
 * *rotate right immediate value by rotation amount
 */
word_t imm_extract(uint16_t op, bool *carry) {
    byte_t imm = op;
    byte_t rotate = 2 * ((op >> 8) & 0xF);
    return shifter(ROR, imm, rotate, carry);
}

/* As a shifted register
 * Assuming - registers involved exclusive of PC and CPSR
 * *splitting :
 *  * bit 3 - 0 : register Rm
 *  * bit 11 - 4 : shift
 * *shift contents of Rm :
 *  * bit 6 - 5 : shift type
 *  * bit 4 == 0 -> shift by constant (bit 11 - 7)
 *  * bit 4 == 1 -> shift by lowest byte in register Rs (bit 11 - 8)
 */
word_t reg_extract(uint16_t op, machine_state* ms, bool *carry) {
    word_t rm = ms->regs.gpr[(op & 0xF)];
    byte_t shift;

    // check bit 4 to assign shift amount
    if ((op >> 4) & 0x1) {
        // shift by register value
        shift = ms->regs.gpr[((op >> 8) & 0xF)];
    } else {
        // shift by constant
        shift = (op >> 7) & 0x1F;
    }

    // shift on shift type
    return shifter(((op >> 5) & 3), rm, shift, carry);

}
