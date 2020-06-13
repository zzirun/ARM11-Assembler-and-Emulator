#include "assemble.h"
#include "assemble_utils.h"

void assemble_mult(program_t *prog, symbol_table_t *st) {

	instr_str_t *instr = prog->curr->instr_str;
	uint32_t result = AL << 28; //Cond field always 1110, S bit always 0
	result += (9 << 4); //Bits 4-7 1001
	
	if (instr->mnemonic == MLA) {
		result += (1 << 21); //Bit A
		result += GET_REG_FROM_STR(instr->operands[3]) << 12; //Rn
	}
	result += GET_REG_FROM_STR(instr->operands[0]) << 16; //Rd
	result += GET_REG_FROM_STR(instr->operands[1]); //Rm
	result += GET_REG_FROM_STR(instr->operands[2]) << 8; //Rs

	free_instr_str(instr);
	prog->curr->binary = result;
}
