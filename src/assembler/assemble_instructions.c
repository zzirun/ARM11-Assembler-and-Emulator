#include "assemble.h"

void assemble_instructions(program_t *prog, symbol_table_t *st) {
  prog->curr = prog->head->next;
  for (; prog->curr != prog->last_instr->next; prog->curr = prog->curr->next) { 
	  prog->curr->instr_str->assemble(prog, st);
  }
}

// Add assemble function for each instruction type
