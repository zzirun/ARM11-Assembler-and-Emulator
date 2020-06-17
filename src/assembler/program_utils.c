#include "assemble_types.h"

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
  char *i = calloc(1, sizeof(char) * (strlen(instr_line) + 1));
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
    prog_elem_t *next = curr->next;
    free(curr);
    curr = next;
  }
  free(program);
}

void free_instr_str(instr_str_t *instr) {
  free(instr->instr_line);
  free(instr->operands);
  free(instr);
}