#include "assemble.h"

void assemble_instructions(program_t *prog, symbol_table_t *st) {
  prog->curr = prog->head->next;
  for (; prog->curr != prog->last_instr->next; prog->curr = prog->curr->next) { 
	  prog->curr->instr_str->assemble(prog, st);
  }
}

/* Data Processing and Special Instruction Assembler */
void assemble_dp(program_t *prog, symbol_table_t *st) {

  /** Translate tokenised form into decoded data processing instruction, 
   *  Encode decoded instruction into binary
   */

  decoded_instr_t dec;
  dec.type = DATA_PROC;
  dec.cond = AL;
  data_processing_t *dp = &dec.dp;

  instr_str_t *instr = prog->curr->instr_str;
  mnemonic_t mnemonic = instr->mnemonic;
  
  if (mnemonic == ANDEQ) {
    dec.cond = EQ;
    dp->opcode = AND_OP;
  } else if (mnemonic == LSL) {
    dp->opcode = MOV_OP;
  } else {
    dp->opcode = mnemonic;
  }

  // Set DP instruction fields
  // get_op_from_str - helper to set immediate and operand 2 fields
  switch (mnemonic) { 
    case AND: 
    case ANDEQ: // Special instr
    case EOR:
    case SUB:
    case RSB:
    case ADD:
    case ORR:
      // instr form: <opcode> Rd, Rn, <Operand2>
      dp->set_cc = 0;
      dp->rd = GET_REG_FROM_STR(instr->operands[0]);
      dp->rn = GET_REG_FROM_STR(instr->operands[1]);
      get_op_from_str(instr->operands[2], &dec); 
      break;
    case LSL: {
      /** Special instr : lsl Rn, <#expression>
       *  * Turn operand 2 from <#expression> to Rn,lsl <#expression>
       *  * Set immediate and operand 2 through helper
       *  * Fall through to mov
       */
      char *rd = instr->operands[0];
      char *expr = instr->operands[1];
      size_t rd_size = strlen(rd) * sizeof(char);
      size_t expr_size = strlen(expr) * sizeof(char);
      // additional 6 characters : ",lsl " and '\0'
      size_t new_size = rd_size + expr_size + 6 * sizeof(char);
      char op2[new_size];
      strncpy(op2, rd, rd_size);
      strncpy(op2 + strlen(rd), ",lsl ", 5 * sizeof(char));
      strncpy(op2 + strlen(rd) + 5, expr, expr_size + sizeof(char)); 
      instr->operands[1] = op2;
      get_op_from_str(instr->operands[1], &dec); 
    }
    case MOV: 
      // instr form: mov Rd, <Operand2>
      dp->set_cc = 0;
      dp->rd = GET_REG_FROM_STR(instr->operands[0]);
      dp->rn = 0; // don't care 
      if (mnemonic == MOV) {
        get_op_from_str(instr->operands[1], &dec); 
      }
      break;
    case TST:
    case TEQ:
    case CMP:
      // instr form: <opcode> Rn, <Operand2>
      dp->set_cc = 1;
      dp->rd = 0; // don't care 
      dp->rn = GET_REG_FROM_STR(instr->operands[0]);
      get_op_from_str(instr->operands[1], &dec);
      break;
    default:
      break;
  } 

  uint32_t bin = ((uint32_t) dec.cond) << 28;
  bin |= ((uint32_t) dp->imm) << 25;
  bin |= ((uint32_t) dp->opcode) << 21;
  bin |= ((uint32_t) dp->set_cc) << 20;
  bin |= ((uint32_t) dp->rn) << 16;
  bin |= ((uint32_t) dp->rd) << 12;
  bin |= ((uint32_t) dp->operand2);

  // Free instruction string
  free_instr_str(instr);
  // Set binary instruction
  prog->curr->binary = bin;
}

/* Multiply Instruction Assembler */
void assemble_mult(program_t *prog, symbol_table_t *st) {

	instr_str_t *instr = prog->curr->instr_str;
  // Condition field always AL
	uint32_t result = AL << 28; 
  // S bit always 0, Bits 4 - 7: 1001
	result += (9 << 4); 
	if (instr->mnemonic == MLA) {
		result |= (1 << 21); // Bit A
		result |= GET_REG_FROM_STR(instr->operands[3]) << 12; // Rn
	} 
	result |= GET_REG_FROM_STR(instr->operands[0]) << 16; // Rd
	result |= GET_REG_FROM_STR(instr->operands[1]); // Rm
	result |= GET_REG_FROM_STR(instr->operands[2]) << 8; // Rs
	
	free_instr_str(instr);
	prog->curr->binary = result;
}

/* Single Data Transfer Instruction Assembler */
void assemble_sdt(program_t *prog, symbol_table_t *st) {
  instr_str_t *instr = prog->curr->instr_str;
  decoded_instr_t dec;
  dec.type = DATA_TRANS;
  dec.cond = AL;

  single_data_transfer_t *sdt = &dec.sdt;
  sdt->u = 1; // assume by default U bit set (+)
  sdt->rd = GET_REG_FROM_STR(instr->operands[0]);
  sdt->imm = 0;
  sdt->p = 0;

  // L bit set when LDR, clear when STR
  sdt->l = (instr->mnemonic == LDR) ? 1 : 0;

  if (instr->operands[1][0] == '=') {
    // Operand as <=expression>
    uint32_t expression = parse_numerical_expr(instr->operands[1], &sdt->u);

    // check value of exp fits into argument of mov
    if (expression <= 0xFF) {
      // assemble instr as a mov instead of an ldr
      instr->mnemonic = MOV;
      instr->operands[1][0] = '#';
      assemble_dp(prog, st);
      return;

    } else {
      // put value of expression at end of assembled program
      sdt->p = 1;
      sdt->rn = PC_REG_NUM; 
      uint16_t address = add_data(prog, expression);
      sdt->offset = address - prog->curr->address - 8;
      
    }
  } else {
    // Operand as pre or post indexed address specification
    char *op = trim(strtok(instr->operands[1], "[]"));
    char *rem = trim(strtok(NULL, ""));

    if (rem) {
      // Post indexed
      sdt->p = 0;
      sdt->rn = GET_REG_FROM_STR(op);
      get_op_from_str(rem, &dec);

    } else {
      // Pre indexed
      sdt->p = 1;
      op = strtok(op, " ,");
      sdt->rn = GET_REG_FROM_STR(op);
      sdt->offset = 0;
      op = trim(strtok(NULL, ""));
      get_op_from_str(op, &dec);
    }
  }

  uint32_t bin = ((uint32_t) dec.cond) << 28;
  bin |= 1 << 26;
  bin |= ((uint32_t) sdt->imm) << 25;
  bin |= ((uint32_t) sdt->p) << 24;
  bin |= ((uint32_t) sdt->u) << 23;
  bin |= ((uint32_t) sdt->l) << 20;
  bin |= ((uint32_t) sdt->rn) << 16;
  bin |= ((uint32_t) sdt->rd) << 12;
  bin |= ((uint32_t) sdt->offset) & 0xFFF;

  free_instr_str(instr);
  prog->curr->binary = bin;
}

/* Branch Instruction Assembler */
void assemble_br(program_t *prog, symbol_table_t *symbol_table) {
  instr_str_t *instr = prog->curr->instr_str;
  // bits 24-27: 1010
  uint32_t bin_instr = 10 << 24;

  uint16_t label_address;
  char *label = instr->operands[0];
  if(!map(symbol_table, label, &label_address)) {
    perror("Label does not exist");
    exit(EXIT_FAILURE);
  }
  uint32_t offset = signed_to_twos_complement((int32_t)label_address - (int32_t)prog->curr->address - 8) >> 2;
  // off by 8 bytes due to ARM pipeline
  offset &= 0xFFFFFF;
  // mask with 0xFFFFFF to get lower 24 bits
  bin_instr |= offset;

  // set the condition codes (cc)
  switch(instr->mnemonic) {
    case BEQ:
      break; // cc : 0000
    case BNE:
      bin_instr |= NE << 28; // cc : 0001
      break;
    case BGE:
      bin_instr |= GE << 28; // cc : 1010
      break;
    case BLT:
      bin_instr |= LT << 28; // cc : 1011
      break;
    case BGT:
      bin_instr |= GT << 28; // cc : 1100
      break;
    case BLE:
      bin_instr |= LE << 28; // cc : 1101
      break;
    case B:
      bin_instr |= AL << 28; // cc : 1110
      break;
    default:
      assert(false);
  }
  free_instr_str(instr);
  prog->curr->binary = bin_instr;
}
