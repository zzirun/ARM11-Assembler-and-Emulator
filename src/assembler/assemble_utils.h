#include "assembletypes.h"
#include "../utils.h"

#define HEXADECIMAL_BASE (16)
#define NUMBER_OF_BYTES_PER_INST (4)

/* Mask to get top 2 bits of a word */
#define GET_MS_2 (0xC0000000) 
/* Mask to get bottom 8 bits of a word */
#define GET_LS_8 (0xFF)

/* Max characters (inc'\0') in a line from assembly file */
#define MAX_LINE_LENGTH (512) 

/* Max representable in immediate field in a dp instr */
#define MAX_DP_IMM (0xFF) 
/* Max representable shift amount in a dp instr */
#define MAX_DP_SHAMT (0xF) 

/* Checks if character is whitespace */
#define IS_WHITESPACE(c) (c == ' ' || c== '\n')

/* Gets register number from string eg: r9 */
#define GET_REG_FROM_STR(reg_as_str) (atoi(reg_as_str + 1))

/* Removes leading and ending whitespace */
char *trim(char *str);
/* Checks if character in string */
int contains(char *s, char c);
/* Converts <#expression> to integer */
bool parse_numerical_expr(char *num_str, uint32_t *num);
/* Converts a hexadecimal string "0x..." to integer */
uint32_t parse_hex(char *hex_str);
/* Converts string to shift type */
shift_type get_shift_type(char *shift_type_str);
/* Converts string <shiftname> <#expression>/<register> to binary shift */
uint8_t parse_shift(char *shift_str);

/* Assigns immediate field and operand2 of a dp instr from a string */
void get_op_from_str(char *op_as_str, decoded_instr_t *instr);
int num_of_operands(char *str);
