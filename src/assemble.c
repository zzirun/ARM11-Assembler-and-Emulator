#include "assembler/assemble.h"
#include "assembler/assemble_utils.h"

int main(int argc, char **argv) {

  symbol_table_t *label_to_address = create_symbol_table();
  instr_list_t *instructions = create_instr_list();

  first_pass(argv[1], label_to_address, instructions);
  tokenizer(instructions);
  assemble_instructions(instructions, label_to_address);
  binary_writer(instructions, argv[2]);

  free_symbol_table(label_to_address);
  free_instr_list(instructions);


	/*
		Just trying to see if writing
		random stuff out to binary works

	FILE* fp;
	uint32_t inst = 3818917889; //11100011101000000001000000000001
	uint32_t inst1 = 3800113154; //11100010100000010010000000000010
	fp = fopen(argv[2], "wb");
	binary_writer(inst, fp);
	binary_writer(inst1, fp);
	fclose(fp);
	*/
}
