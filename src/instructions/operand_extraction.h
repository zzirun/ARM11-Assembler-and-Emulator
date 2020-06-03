#ifndef OPEXTRACT_H
#define OPEXTRACT_H

// operand as rotated immediate
uint32_t imm_extract(uint16_t op, bool *carry);

// operand as shifted register contents
uint32_t reg_extract(uint16_t op, machine_state* ms, bool *carry);

#endif
