#include "assemble.h"
#include "assemble_utils.h"

void assemble_instructions(instr_list_t * instructions, symbol_table_t *table) {
  instr_t *curr = instructions->head->next;
  for (; curr; curr = curr->next) { 
	  tokenized_instr_t *instr = curr->tokenized_instr;
	  instr->assemble(instr, table);
  }
}

// Add assemble function for each instruction type