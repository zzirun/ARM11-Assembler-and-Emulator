#include "assembletypes.h"

void free_tokenized_instr(tokenized_instr_t *instr) {
  for (int i = 0; i < instr->no_of_operands; i++) {
    free(instr->operands[i]);
  }
  free(instr->operands);
  free(instr);
}

symbol_table_t *create_symbol_table(void) {
  symbol_table_t *table = calloc(1, sizeof(symbol_table_t));
  symbol_table_elem_t *elem = calloc(1, sizeof(symbol_table_elem_t));
  if (!table || !elem) {
    perror("Failed Memory Allocation for Symbol Table");
    exit(EXIT_FAILURE);
  }
  table->head = elem;
  table->tail = elem;
  return table;
}

void add_mapping(symbol_table_t *table, const char *label, uint16_t address) {
  symbol_table_elem_t *elem = calloc(1, sizeof(symbol_table_elem_t));
  char *l = calloc(1, sizeof(strlen(label) + 1));
  if (!elem || !l) {
      perror("Failed Memory Allocation for Symbol Table Element");
      exit(EXIT_FAILURE);
  }
  strcpy(l, label);
  elem->label = l;
  elem->address = address;
  
  table->tail->next = elem;
  table->tail = elem; 
}

bool map(symbol_table_t *table, const char *label, uint16_t *address) {
  symbol_table_elem_t *curr = table->head->next;
  for (; curr; curr = curr->next) {
    if (!strcmp(label, curr->label)) {
      *address = curr->address;
      return true;
    }
  }
  return false;
}

void free_symbol_table(symbol_table_t *table) {
  symbol_table_elem_t *curr = table->head;
  while (curr) {
    symbol_table_elem_t *next = curr->next;
    free(curr->label);
    free(curr);
    curr = next;
  }
  free(table);
}

instr_list_t *create_instr_list(void) {
  instr_list_t *instr_list = calloc(1, sizeof(instr_list_t));
  instr_t *instr = calloc(1, sizeof(instr_t));
  if (!instr_list || !instr) {
    perror("Failed Memory Allocation for Instructions List");
    exit(EXIT_FAILURE);
  }
  instr_list->head = instr;
  instr_list->tail = instr;
  return instr_list;
}

void add_instr(instr_list_t *instr_list, const char *inst, uint16_t address) {
  instr_t *instr = calloc(1, sizeof(instr_t));
  char *i = calloc(1, sizeof(strlen(inst) + 1));
  if (!instr || !i) {
      perror("Failed Memory Allocation for Instruction");
      exit(EXIT_FAILURE);
  }
  strcpy(i, inst);
  instr->instr_str = i;
  instr->address = address;
  
  instr_list->tail->next = instr;
  instr_list->tail = instr; 
}

// freed after binary file writing
// string field should have already been freed
void free_instr_list(instr_list_t *instr_list) {
  instr_t *curr = instr_list->head;
  while (curr) {
    instr_t *next  = curr->next;
    free(curr);
    curr = next;
  }
  free(instr_list);
}
