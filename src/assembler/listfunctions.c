#include "assembletypes.h"

void free_instr_str(instr_str_t *instr) {
  free(instr->instr_line);
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

program_t *create_program(void) {
  program_t *program = calloc(1, sizeof(program_t));
  prog_elem_t *prog_elem = calloc(1, sizeof(prog_elem_t));
  if (!program || !prog_elem) {
    perror("Failed Memory Allocation for Program");
    exit(EXIT_FAILURE);
  }
  program->head = prog_elem;
  program->tail = prog_elem;
  return program;
}

void add_instr(program_t *program, const char *instr_line, uint16_t address) {
  instr_t *instr = calloc(1, sizeof(instr_t));
  instr_str_t *instr_str = calloc(1, sizeof(instr_str_t));
  char *i = calloc(1, sizeof(strlen(instr_line) + 1));
  if (!instr || !instr_str || !i) {
      perror("Failed Memory Allocation for Instruction");
      exit(EXIT_FAILURE);
  }
  strcpy(i, instr_line);
  instr_str->instr_line = i;
  instr->instr_str = instr_str;
  instr->address = address;
  
  program->tail->next = instr;
  program->tail = instr; 
}

// adds value to end of assembled program, returns address of value
uint16_t add_data(program_t *program, uint32_t value) {
  data_t *data = calloc(1, sizeof(data_t));
  if (!data) {
    perror("Failed Memory Allocation for Extra Value");
    exit(EXIT_FAILURE);
  }
  data->binary = value;
  data->address = program->tail->address + 4;

  program->tail->next = data;
  program->tail = data; 

  return data->address;
}

void free_program(program_t *program) {
  prog_elem_t *curr = program->head;
  while (curr) {
    prog_elem_t *next  = curr->next;
    free(curr);
    curr = next;
  }
  free(program);
}
