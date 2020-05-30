#include <stdint.h>
#include "../utils.h"
#include "../types.h"
#include "operandExtraction.h"

/* returns operand2 for DP, offset for SDT */


word_t immExtract(uint16_t op, bool *carry) {
  // split into rotation (bit 11 -8) and unsigned byte imm (bit 7 - 0)
  byte_t imm = op; // cuts off top 8 bits
  byte_t rotate = 2 * ((op >> 8) & 0xF); //extractBits(op, 8, 11)

  // rotateRight
  return shifter(ROR, imm, rotate, carry);
}

// ASSUMING EXCLUSIVE OF PC AND CPSR <- CHECK
word_t regExtract(uint16_t op, machine_state* ms, bool *carry) {
  // split to shift (bit 11 - 4) and register (bit 3 - 0)
  // shift type : bit 6 - 5
  // if bit 4 = 0, shift by constant (bits 11 - 7)
  // if bit 4 = 1, shift by bottom byte of register (bits 11 - 8) contents

  word_t extracted = ms->regs.gpr[(op & 0x7)]; //extractBits(op, 0, 3)
  byte_t shift;

  // check bit 4 to assign shift amount
  if ((op >> 4) & 0x1) { //extractBits(op, 4, 4)
    // shift by register value
    shift = ms->regs.gpr[((op >> 8) & 0xF)]; //extractBits(op, 8, 11)
  } else {
    // shift by constant
    shift = (op >> 7) & 0x1F; //extractBits(op, 7, 11)
  }

  // shift on shift type
  return shifter(((op >> 5) & 3), extracted, shift, carry); //extractBits(op, 5, 6)

}
