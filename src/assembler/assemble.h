#include "assembletypes.h"

void first_pass(char *file_path, symbol_table_t *st, program_t *prog);

void tokenizer(program_t *);

void assemble_instructions(program_t *, symbol_table_t *);
void assemble_dp(program_t *, symbol_table_t *); 
void assemble_mult(program_t *, symbol_table_t *); 
void assemble_sdt(program_t *, symbol_table_t *); 
void assemble_br(program_t *, symbol_table_t *); 

void binary_writer(program_t *prog, char *file_path);
