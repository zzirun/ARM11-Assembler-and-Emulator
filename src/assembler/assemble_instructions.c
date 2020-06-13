#include "assemble.h"
#include "assemble_utils.h"

void assemble_instructions(instr_list_t * il, symbol_table_t *st) {
  il->curr = il->head->next;
  for (; il->curr; il->curr = il->curr->next) { 
	  il->curr->tokenized_instr->assemble(il, st);
  }
}

// Add assemble function for each instruction type