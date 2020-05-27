#include <stdint.h>
#include "../utils.h"
#include "operandExtraction.h"

/* returns operand2 for DP, offset for SDT */


uint32_t immExtract(uint16_t op, bool *carry) {
  // split into rotation (bit 11 -8) and unsigned byte imm (bit 7 - 0)
  uint8_t imm = op; // cuts off top 8 bits
  uint8_t rotate = 2 * extractBits(op, 8, 11);

  // rotateRight
  return shifter(ROR, imm, rotate, carry);
}

// ASSUMING EXCLUSIVE OF PC AND CPSR <- CHECK
uint32_t regExtract(uint16_t op, registers *regs, bool *carry) {
  // split to shift (bit 11 - 4) and register (bit 3 - 0)
  // shift type : bit 6 - 5
  // if bit 4 = 0, shift by constant (bits 11 - 7)
  // if bit 4 = 1, shift by bottom byte of register (bits 11 - 8) contents

  uint32_t extracted = regs -> gpr[extractBits(op, 0, 3)];
  uint8_t shift;

  // check bit 4 to assign shift amount
  if (extractBits(op, 4, 4)) {
    // shift by register value
    shift = regs -> gpr[extractBits(op, 8, 11)]
  } else {
    // shift by constant
    shift = extractBits(op, 7, 11);
  }

  // shift on shift type
  return shifter(extractBits(op, 5, 6), extracted, shift, carry);

}
