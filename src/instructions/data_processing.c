#include <stdio.h>
#include <stdbool.h>
#include "../types.h"
#include "operandExtraction.h"
#include "../utils.h"
#include "datafunctions.h"

void dataProc(machine_state *ms) {

  word_t op1 = ms->regs.gpr[ms->instrToExecute.rn];
  word_t op2;
  word_t result;
  byte_t opcode = ms->instrToExecute.operation_code;

  // logical op (AND, EOR, ORR, TEQ, TST, MOV) : shifter carry
  // arithmetic op (ADD, SUB, RSB, CMP) : ALU carry
  bool carry = 0;
  
  // Assign second operand
  // Set all carry to shifter carry
  if (ms->instrToExecute.imm) {
    // rotated 8-bit Imm
    op2 = immExtract(ms->instrToExecute.operand_offset, &carry);
  } else {
    //shifted reg
    op2 = regExtract(ms->instrToExecute.operand_offset, ms, &carry);
  }

  // Arithmetic/logic operation based on opcode
  // Set carry for arithmetic op (ADD, SUB, CMP, RSB) to ALU carry
  switch (opcode) {
    case AND:
    case TST:
      result = op1 & op2;
      break;
    case EOR:
    case TEQ:
      result = op1 ^ op2;
      break;
    case SUB:
    case CMP:
      result = op1 - op2;
      // set carry if no borrow
      carry = op1 < op2 ? 0 : 1;
      break;
    case RSB:
      result = op2 - op1;
      // set carry if no borrow
      carry = op2 < op1 ? 0 : 1;
      break;
    case ADD:
      result = op1 + op2;
      // set carry if unsigned overflow
      carry = result < op1 ? 1 : 0;
      break;
    case ORR:
      result = op1 | op2;
      break;
    case MOV:
      result = op2;
      break;
    default:
      fprintf(stderr, "Invalid Data Processing Opcode");
      terminate(ms);
  }

  // Write to destination for specific operations
  switch (opcode) {
    case TST:
    case TEQ:
    case CMP:
      // don't write
      break;
    default:
      // destReg = result
      ms->regs.gpr[ms->instrToExecute.rd] = result;
  }

  /* Update CPSR flags if S bit set
   * CPSR - bit 31 - 28 : NZCV
     * V unaffected
     * C set to carry (ALU/Shifter):
     * Z set only if result all 0s
     * N set to logical bit 31 of result
   */
  if (ms->instrToExecute.set_cc) {
    word_t flagsNew = (C * carry) | (Z * (result == 0)) | (N * (result >> 31));
    // clear top 3 bits and set to new flags
    ms->regs.CPSR = (ms->regs.CPSR & 0x1FFFFFFF) | (flagsNew << 28);
  }

}
