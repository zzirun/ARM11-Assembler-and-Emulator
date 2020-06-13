#include "assembletypes.h"

void first_pass(char *file_path, symbol_table_t *s_t, instr_list_t *i_l);

void tokenizer(instr_list_t *);

void assemble_instructions(instr_list_t *, symbol_table_t *);
void assemble_dp(instr_list_t *, symbol_table_t *); 
void mult_assembly(instr_list_t *, symbol_table_t *); 
void assemble_sdt(instr_list_t *, symbol_table_t *); 
void assemble_br(instr_list_t *, symbol_table_t *); 

void binary_writer(instr_list_t *, char *file_path);