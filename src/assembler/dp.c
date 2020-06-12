#include "assemble.h"

void assemble_dp(instr_t *instr) {

  /* Translate tokenised form into data processing instruction */
  data_processing_t dp;
  tokenized_instr_t *tok_instr = &(instr->tokenised_instr);

  mnemonic_t mnemonic = tok_instr->mnemonic;
  dp.opcode = mnemonic;
  switch (mnemonic) { 
    case AND:
    case EOR:
    case SUB:
    case RSB:
    case ADD:
    case ORR:
      // compute results
      // form: <opcode> Rd, Rn, <Operand2>
      dp.set_cc = 0;
      dp.rd = GET_REG_FROM_STR(tok_instr->operands[0]);
      dp.rn = GET_REG_FROM_STR(tok_instr->operands[1]);
      get_op_from_str(tok_instr->operands[2], &dp); // sets imm and operand2
      break;
    case MOV:
      // single operand assignment
      // form: mov Rd, <Operand2>
      dp.set_cc = 0;
      dp.rd = GET_REG_FROM_STR(tok_instr->operands[0]);
      dp.rn = 0; // don't care (below 4 bits)
      get_op_from_str(tok_instr->operands[1], &dp); // sets imm and operand2
      break;
    case TST:
    case TEQ:
    case CMP:
      // testing instrs: set CPSR flags, don't compute results
      // form: <opcode> Rn, <Operand2>
      dp.set_cc = 1;
      dp.rd = 0; // don't care (below 4 bits)
      dp.rn = GET_REG_FROM_STR(tok_instr->operands[0]);
      get_op_from_str(tok_instr->operands[1], &dp); // sets imm and operand2
      break;
  } 

  /* Encode decoded instruction into binary */
  uint32_t bin = 0;
  // bit 31 - 28 : condition code (always)
  bin |= AL;
  // bit 27 - 26 : 00
  // bit 25 : immediate flag
  bin = (bin << 3) | dp.imm;
  // bit 24 - 21 : opcode
  bin = (bin << 4) | dp.opcode;
  // bit 20 : set condition codes flag
  bin = (bin << 1) | dp.set_cc;
  // bit 19 - 16 : Rn
  bin = (bin << 4) | dp.rn;
  // bit 15 - 12 : Rd
  bin = (bin << 4) | dp.rd;
  // bit 11 - 0 : operand 2
  bin = (bin << 12) | dp.operand2;
  
  /* Set binary instruction */
  instr->binary_instr = bin;
}



