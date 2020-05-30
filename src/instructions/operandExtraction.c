#include <stdint.h>
#include "../utils.h"
#include "../types.h"
#include "operandExtraction.h"

<<<<<<< HEAD
/* Returns operand2 for DP, offset for SDT */

/* As a rotated 8-bit immediate constant 
 * *splitting :
 *  * bit 7 - 0 : unsigned immediate value (byte)
 *  * bit 11 -8 : half of rotation amount
 * *rotate right immediate value by rotation amount 
 */
word_t immExtract(uint16_t op, bool *carry) {
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
word_t regExtract(uint16_t op, machine_state* ms, bool *carry) {
  word_t extracted = ms->regs.gpr[(op & 0x7)]; //extractBits(op, 0, 3)
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
