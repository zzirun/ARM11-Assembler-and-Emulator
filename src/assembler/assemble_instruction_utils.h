#include "assemble_types.h"

#define NUMBER_OF_BYTES_PER_INST (4)
#define HEXADECIMAL_BASE (16)
/* Decimal value of hex character 'A' */
#define A_AS_DECIMAL (10)

/* Mask to get bottom 8 bits of a word */
#define GET_LS_8 (0xFF)

/* Max characters (includes '\0') in a line from assembly file */
#define MAX_LINE_LENGTH (512) 

/* Max representable in immediate field in a dp instr */
#define MAX_DP_IMM (0xFF) 
/* Max representable shift amount in a dp instr */
#define MAX_DP_SHAMT (0xF) 

/* Checks if character is to be removed : space, newline, comma */
#define TO_REMOVE(c) (c == ' ' || c == '\n' || c == ',')
/* Checks if character is in a consecutive range of characters */
#define IN_RANGE(c, start, end) (c >= start && c <= end)

/* Gets register number from string eg: input r9 returns 9 */
#define GET_REG_FROM_STR(reg_as_str) (atoi(reg_as_str + 1))

/* Removes leading and ending whitespace and commas */
char *trim(char *str);
/* Checks if character in string */
bool contains(char *s, char c);
/* Counts number of operands, separated by commas, in a string */
int num_of_operands(char *str);
/* Converts string <#/=expression> to integer magnitude and sign */
uint32_t parse_numerical_expr(char *num_str, bool *sign);
/* Converts a hexadecimal string "0x..." to integer (unsigned) */
uint32_t parse_hex(char *hex_str);
/* Converts string to shift type */
shift_t get_shift_type(char *shift_type_str);
/* Encodes string to binary shift */
uint8_t parse_shift(char *shift_str);

/* Assigns immediate flag, operand2/offset of a DP/SDT instr from a string */
void get_op_from_str(char *op_as_str, decoded_instr_t *instr);
/* Converts a signed integer to its twos complement form */
uint32_t signed_to_twos_complement(int32_t value);