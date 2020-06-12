#include "assembletypes.h"
#include "../utils.h"

#define GET_REG_FROM_STR(reg_as_str) (atoi(reg_as_str + 1))

void assemble_dp(instr_t *instr);
void get_op_from_str(char *op_as_str, data_processing_t *dp);
int parse_numerical_expr(char *num_str);
int parse_hex(char *hex_str);
uint32_t encode_dp(decoded_instr_t *dec);