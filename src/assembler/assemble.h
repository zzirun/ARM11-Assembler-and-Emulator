#include "assembletypes.h"

void first_pass(char *file_path, symbol_table_t *s_t, instr_list_t *i_l);

void tokenizer(instr_list_t *);

void assemble_instructions(instr_list_t *, symbol_table_t *);
uint32_t assemble_dp(tokenized_instr_t *, symbol_table_t *); 

void binary_writer(instr_list_t *, char *file_path);