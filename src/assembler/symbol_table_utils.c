#include "assemble_types.h"

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
  char *l = calloc(sizeof(char), strlen(label) + 1);
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
