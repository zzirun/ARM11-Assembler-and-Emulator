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
  free_instr_list(program);


	/*
		Just trying to see if writing
		random stuff out to binary works

	FILE* fp;
	fp = fopen(argv[2], "wb");
	//mul r3,r1,r2
	tokenized_instr_t* test = calloc(1, sizeof(tokenized_instr_t));
	test->func = &mult_assembly;
	test->mnemonic = MUL;
	char* r3 = "r3";
	char* r1 = "r1";
	char* r2 = "r2";
	test->operands = calloc(3, sizeof(char*));
	test->operands[0] = r3;
	test->operands[1] = r1;
	test->operands[2] = r2;
	binary_writer(test->func(test), fp);
	free(test->operands);
	free(test);
	fclose(fp);
	*/
}
