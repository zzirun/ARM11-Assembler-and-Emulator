#include "dp.h"

#define GET_REG_FROM_STR(reg_as_str) (atoi(reg_as_str + 1))

void assemble_dp(instr_t *instr) {
  decoded_instr_t dec;
  dec.type = DATA_PROC;
  dec.cond = AL;
  data_processing_t dp;
  
  tokenized_instr_t tok_instr = instr->tokenised_instr;

  mnemonic_t mnemonic = tok_instr.mnemonic;
  dp.opcode = mnemonic;
  switch (mnemonic) { 
    case AND:
    case EOR:
    case SUB:
    case RSB:
    case ADD:
    case ORR:
      // compute results
      // <opcode> Rd, Rn, <Operand2>
      dp.set_cc = 0;
      dp.rd = GET_REG_FROM_STR(tok_instr.operands[0]);
      dp.rn = GET_REG_FROM_STR(tok_instr.operands[1]);
      get_op_from_str(tok_instr.operands[2], &dp);
      break;
    case MOV:
      // single operand assignment
      // mov Rd, <Operand2>
      dp.set_cc = 0;
      dp.rd = GET_REG_FROM_STR(tok_instr.operands[0]);
      get_op_from_str(tok_instr.operands[1], &dp);
      break;
    case TST:
    case TEQ:
    case CMP:
      // testing instrs: set CPSR flags, don't compute results
      // <opcode> Rn, <Operand2>
      dp.set_cc = 1;
      dp.rn = GET_REG_FROM_STR(tok_instr.operands[0]);
      get_op_from_str(tok_instr.operands[1], &dp);
      break;
  }
  instr->binary_instr = encode_dp(&dec);
}



