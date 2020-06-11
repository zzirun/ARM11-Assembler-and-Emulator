#include "assembletypes.h"

uint32_t mult_assembly(tokenized_instr_t* instr) {
	uint32_t result = 14 << 28; //Cond field always 1110, S bit always 0
	result += (9 << 4); //Bits 4-7 1001
	if (instr->mnemonic == MLA) {
		result += (1 << 21); //Bit A
		result += atoi(&(instr->operands[3])[1]) << 12; //Rn
	}
	result += atoi(&(instr->operands[0])[1]) << 16; //Rd
	result += atoi(&(instr->operands[1])[1]); //Rm
	result += atoi(&(instr->operands[2])[1]) << 8; //Rs
	return result;
}
