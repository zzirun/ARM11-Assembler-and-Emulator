#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "assembler/listfunctions.c"

#define MAX_LINE_LENGTH (512) //Assumption made according to the spec
#define NUMBER_OF_BYTES_PER_INST (4)

/*
	Writes 32 bits instruction into stream according to little endian format
*/

void first_pass(char* path_to_file, symbol_table_t* s_t, instr_list_t* i_l) {
	char buffer[MAX_LINE_LENGTH] = {0};
	FILE* assembly_file = fopen(path_to_file, "r");
	if (!assembly_file) {
		perror("Failed to open source code");
		exit(EXIT_FAILURE);
	}
	uint16_t address = 0;
	while (fgets(&buffer[0], sizeof(buffer), assembly_file)) {
		bool is_label = false;
		for (int i = 0; buffer[i]; i++) {
			if (buffer[i] == ':') {
				buffer[i] = 0; //Removes ':' from the label
				is_label = true;
				break;
			}
		}
		if (is_label) {
			add_mapping(s_t, buffer, address);
		} else {
			add_instr(i_l, buffer);
		}
		address += 4;
	}
	fclose(assembly_file);
}

void binary_writer(uint32_t instruction, FILE* stream) {
	uint8_t inst_arr[NUMBER_OF_BYTES_PER_INST];
	uint32_t mask = 255; //1111111
	for (int i = 0; i < NUMBER_OF_BYTES_PER_INST; i++) {
		inst_arr[i] = instruction & mask;
		instruction >>= 8;
	}
	fwrite(&inst_arr[0], sizeof(uint8_t), 4, stream);
}

int main(int argc, char **argv) {

	symbol_table_t* s_t = calloc(1, sizeof(symbol_table_t));
	instr_list_t* i_l = calloc(1, sizeof(instr_list_t));

	first_pass(argv[1], s_t, i_l);

	free_symbol_table(s_t);
	free_instr_list(i_l);


	/*
		Just trying to see if writing
		random stuff out to binary works
	*/
	FILE* fp;
	uint32_t inst = 3818917889; //11100011101000000001000000000001
	uint32_t inst1 = 3800113154; //11100010100000010010000000000010
	fp = fopen(argv[2], "wb");
	binary_writer(inst, fp);
	binary_writer(inst1, fp);
	fclose(fp);
}
