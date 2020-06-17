#include "assemble_types.h"

/* Constructs label to address map, loads instructions from assembly file */
void first_pass(char *input_file_path, symbol_table_t *, program_t *);

/** All whole instruction strings converted to tokenized form
 *  (mnemonic type, assemble function, operands)
 */
void tokenizer(program_t *);

/** All tokenized instructions in a program converted to binary,
 *  Each instruction is passed to a function corresponding to its type
 */
void assemble_instructions(program_t *, symbol_table_t *);
void assemble_dp(program_t *, symbol_table_t *); 
void assemble_mult(program_t *, symbol_table_t *); 
void assemble_sdt(program_t *, symbol_table_t *); 
void assemble_br(program_t *, symbol_table_t *); 

/* Writes all assembled binary instructions into specified file */
void binary_writer(program_t *, char *output_file_path);

