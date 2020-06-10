#include "assembletypes.h"

symbol_table_t *symbol_table_new() {
  symbol_table_t *elem = calloc(1, sizeof(symbol_table_t));
  if (!elem) {
    perror("Failed Memory Allocation for Symbol Table");
    exit(EXIT_FAILURE);
  }
  return elem;
}

void *symbol_table_add(symbol_table_t *table, const char *label, uint16_t address) {
  symbol_table_elem_t *elem = calloc(1, sizeof(symbol_table_elem_t));
  char *l = calloc(1, sizeof(strlen(label) + 1));
  if (!elem || !l) {
      perror("Failed Memory Allocation for Symbol Table Element");
      exit(EXIT_FAILURE);
  }
  strcpy(l, label);
  elem->label = l;
  elem->address = address;
  
  if (table->head && table->tail) {
    // elements > 1
    table->tail->next = elem;
    table->tail = elem; 
  } else {
    // where first element is added
    table->head = elem;
    table->tail = elem;
  }
  // REDUNDANCY WITH INSTR LIST ADD
}

static void free_st_elem(symbol_table_elem_t *elem) {
  if (elem) {
    free_st(elem->label);
    free_st(elem->next);
  }
  free(elem);
}

void free_symbol_table(symbol_table_t *table) {
  free_st(table->head);
  free(table);
}

instr_list_t *instr_list_new() {
    instr_list_t *instr_list = calloc(1, sizeof(instr_list_t));
    if (!instr_list) {
        perror("Failed Memory Allocation for Symbol Table");
        exit(EXIT_FAILURE);
    }
    return instr_list;
}

void *instr_list_add(instr_list_t *instr_list, const char *instr_str) {
  instr_t *instr = calloc(1, sizeof(instr_t));
  char *i = calloc(1, sizeof(strlen(instr_str) + 1));
  if (!instr || !i) {
      perror("Failed Memory Allocation for Instruction");
      exit(EXIT_FAILURE);
  }
  strcpy(i, instr_str);
  instr->instr_str = i;
  
  if (instr_list->head && instr_list->tail) {
    // instr > 1
    instr_list->tail->next = instr;
    instr_list->tail = instr; 
  } else {
    // where first instr is added
    instr_list->head = instr;
    instr_list->tail = instr;
  }
  // REDUNDANCY WITH SYMBOL TABLE ADD
}

// freed after binary file writing
// string field should have already been freed
static void free_il(instr_t *instr) {
  if (instr) free_instr_list(instr->next);
  free(instr);
}

void free_instr_list(instr_list_t *instr_list) {
  free_il(instr_list->head);
  free(instr_list);
}
    
