#include "assemble.h"
#include "assemble_instruction_utils.h"

void assemble_sdt(program_t *prog, symbol_table_t *st) {
  instr_str_t *instr = prog->curr->instr_str;
  decoded_instr_t dec;
  dec.type = DATA_TRANS;
  dec.cond = AL;

  single_data_transfer_t *sdt = &dec.sdt;
  sdt->u = 1; // assume by default set (+)
  sdt->rd = GET_REG_FROM_STR(instr->operands[0]);
  sdt->imm = 0;
  sdt->p = 0;

  // L bit set when LDR, clear when STR
  sdt->l = (instr->mnemonic == LDR) ? 1 : 0;

  if (instr->operands[1][0] == '=') {
    /* Operand as <=expression> */
    // Find value of expression
    word_t expression = parse_numerical_expr(instr->operands[1], &sdt->u);

    // Check value of exp fits into argument of mov
    if (expression <= 0xFF) {
      // Assemble instr as a mov instead of an ldr
      instr->mnemonic = MOV;
      instr->operands[1][0] = '#';
      assemble_dp(prog, st);
      return;

    } else {
      // Put value of expression at end of assembled program
      sdt->p = 1;
      sdt->rn = 15; //register number of PC
      uint16_t address = add_data(prog, expression);
      sdt->offset = address - prog->curr->address - 8;
      
    }
  } else {
    // pre or post indexed
    char *op = trim(strtok(instr->operands[1], "[]"));
    char *rem = trim(strtok(NULL, ""));

    if (rem) {
      // post indexing
      sdt->p = 0;
      sdt->rn = GET_REG_FROM_STR(op);
      get_op_from_str(rem, &dec);

    } else {
      // pre indexing
      sdt->p = 1;
      op = strtok(op, " ,");
      sdt->rn = GET_REG_FROM_STR(op);
      sdt->offset = 0;
      op = trim(strtok(NULL, ""));
      get_op_from_str(op, &dec);
    }
  }

  word_t bin = ((word_t) AL) << 28;
  bin |= 1 << 26;
  bin |= ((word_t) sdt->imm) << 25;
  bin |= ((word_t) sdt->p) << 24;
  bin |= ((word_t) sdt->u) << 23;
  bin |= ((word_t) sdt->l) << 20;
  bin |= ((word_t) sdt->rn) << 16;
  bin |= ((word_t) sdt->rd) << 12;
  bin |= ((word_t) sdt->offset) & 0xFFF;

  free_instr_str(instr);
  prog->curr->binary = bin;
}
