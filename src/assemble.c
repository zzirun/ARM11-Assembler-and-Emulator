#include "assembler/assemble.h"
#include "assembler/assemble_utils.h"

int main(int argc, char **argv) {

  symbol_table_t *label_to_address = create_symbol_table();
  program_t *program = create_program();

  first_pass(argv[1], label_to_address, program);
  tokenizer(program);
  assemble_instructions(program, label_to_address);
  binary_writer(program, argv[2]);

  free_symbol_table(label_to_address);
  free_program(program);

}
