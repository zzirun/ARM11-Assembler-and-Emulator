#include "assembletypes.h"
#include "../utils.h"

#define HEXADECIMAL_BASE (16)

/* Mask to get top 2 bits of a word */
#define GET_MS_2 (0xC0000000) 

/* Max representable in immediate field in a dp instr */
#define MAX_DP_IMM (0xFF) 
/* Max representable shift amount in a dp instr */
#define MAX_DP_SHAMT (0xF) 

/* Gets register number from string eg: r9 */
#define GET_REG_FROM_STR(reg_as_str) (atoi(reg_as_str + 1))

/* Converts <#expression> to integer */
uint32_t parse_numerical_expr(char *num_str);
/* Converts a hexadecimal string "0x..." to integer */
uint32_t parse_hex(char *hex_str);
/* Converts string to shift type */
shift_type get_shift_type(char *shift_type_str);
/* Converts string <shiftname> <#expression>/<register> to binary shift */
uint8_t parse_shift(char *shift_str);

/* Assigns immediate field and operand2 of a dp instr from a string */
void get_op_from_str(char *op_as_str, data_processing_t *dp);