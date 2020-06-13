#include "assemble.h"
#include "assemble_utils.h"

void assemble_instructions(program_t *prog, symbol_table_t *st) {
  prog->curr = prog->head->next;
	//prog->curr != prog->last_instr->next
	//Brandon : Changed the condition here just to get it running, not sure if correct according to what you guys had in mind
  for (; prog->curr != prog->tail; prog->curr = prog->curr->next) {
	  prog->curr->instr_str->assemble(prog, st);
  }
	prog->curr->instr_str->assemble(prog, st);
}

// Add assemble function for each instruction type
