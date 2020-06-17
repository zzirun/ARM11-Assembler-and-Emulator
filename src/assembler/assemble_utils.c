#include "assemble.h"

/** Parses assembly file to :
 *  * build label to address map 
 *  * load non-label instruction strings
 */
void first_pass(char *file_path, symbol_table_t *st, program_t *prog) {
  char buffer[MAX_LINE_LENGTH] = {0};
  FILE *assembly_file = fopen(file_path, "r");
  if (!assembly_file) {
    perror("Failed to open source code");
    exit(EXIT_FAILURE);
  }
  uint16_t address = 0;
  while (fgets(buffer, MAX_LINE_LENGTH, assembly_file)) {
    char *trim_buffer = trim(buffer);
    if (trim_buffer[0] != '\0') {
      bool is_label = false;
      for (int i = 0; buffer[i]; i++) {
        if (buffer[i] == ':') {
          buffer[i] = '\0'; //Removes ':' from the label
          is_label = true;
          break;
        }
      }
      if (is_label) {
        add_mapping(st, trim_buffer, address);
      } else {
        add_instr(prog, trim_buffer, address);
        address += 4;
      }
    }
  }
  fclose(assembly_file);
  prog->last_instr = prog->tail;
}

/* Writes 32-bit instructions into file according to little endian format */
void binary_writer(program_t *program, char *file_path) {
  FILE *binary_file = fopen(file_path, "wb");
  if (!binary_file) {
    perror("Failed to open binary file");
    exit(EXIT_FAILURE);
  }
  instr_t *curr = program->head->next;
  uint8_t inst_arr[NUMBER_OF_BYTES_PER_INST];
  for (; curr; curr = curr->next) {
    uint32_t instr = curr->binary;
    for (int i = 0; i < NUMBER_OF_BYTES_PER_INST; i++) {
      inst_arr[i] = instr & GET_LS_8;
      instr >>= BYTE_SIZE;
    }
    fwrite(inst_arr, sizeof(uint8_t), NUMBER_OF_BYTES_PER_INST, binary_file);
  }
  fclose(binary_file);
}
