#include "dp.h"

/**
 *  Parses string represention of a numerical constant
 *  <#expression> into an integer
 **/
int parse_numerical_expr(char *num_str) {
  int num = 0;
  // parse as either hexadecimal or decimal by 
  // checking for prefix "0x"
  if (num_str[0] == '0' && num_str[1] == 'x') {
    // hexadecimal, skip over first 2 characters 
    num = parse_hex(num_str + 2);
  } else {
    // decimal
    num = atoi(num_str);
  }
  return num;
}

#define HEXADECIMAL_BASE (16)

// Parses a hexadecimal string into an integer
int parse_hex(char *hex_str) {
  int num = 0;
  for (int i = 0; i < strlen(hex_str); i++) {
    num *= HEXADECIMAL_BASE;
    char c = hex_str[i];
    if (c >= 'A' && c <= 'F') {
      num += c - 'A' + 10;
    } else if (c >= '0' && c <= '9') {
      num += c - '0';
    } else {
      fprintf(stderr, "Invalid Character in Instruction");
      exit(EXIT_FAILURE);
    }
  }
  return num;
}

shift_type get_shift_type(char *shift_type_str) {
  if (!strcmp("lsl", shift_type_str)) return LSL;
  if (!strcmp("lsr", shift_type_str)) return LSR;
  if (!strcmp("asr", shift_type_str)) return ASR;
  if (!strcmp("ror", shift_type_str)) return ROR;
  
  fprintf(stderr, "Invalid Shift Type");
  exit(EXIT_FAILURE);
}

uint8_t parse_shift(char *shift_str) {
  // No shift
  if (!shift_str) return 0;
  // Has shift
  char *shift_field = strtok(shift_str, " ");
  uint8_t shift_t = get_shift_type(shift_field);
  shift_field = strtok(NULL, "");
  bool shift_by_reg;
  uint8_t shift_amount;
  if (*shift_field == '#') {
    // shift by constant amount <#expression>
    shift_by_reg = 0;
    shift_amount = parse_numerical_expr(shift_field);
    shift_amount <<= 3;
  } else if (*shift_field == 'r') {
    // shift by bottom byte of register <register>
    shift_by_reg = 1;
    shift_amount = GET_REG_FROM_STR(shift_field);
    shift_amount <<= 4
  } else {
    fprintf(stderr, "Invalid Shift Operand");
    exit(EXIT_FAILURE);
  }
  shift_t <<= 1;
  // build binary representation of shift  
  return shift_amount | shift_t | shift_by_reg;
}


#define MAX_IMM (0xFF)
#define MAX_SHAMT (0xF)
#define GET_MS_2 (0xC0000000) // mask to get top 2 bits of a word

// TO DO : ADD ERRORS FOR INVALID CASES
void get_op_from_str(char *op_as_str, data_processing_t *dp) {
  dp->imm = *op_as_str == '#';
  uint16_t op2 = 0;
  if (dp->imm) {
    /*  Operand2 as <#expression>
     *  1. get number to represent
     *  2. convert to representation if possible: 
     *     8-bit unsigned immediate + 4-bit ROR shift amount 
     *     max imm = 0xFF = 255 , max shamt = 0xF X 2 = 30
     */
    // 1 :
    uint32_t num = parse_numerical_expr(op_as_str + 1); 
    // 2 : reverse ROR until fit into 8-bit or exceed max shamt
    uint8_t shift = 0;
    while (num > MAX_IMM) {
      if (++shift > MAX_SHAMT) {
        fprintf(stderr, "Operand Cannot Be Represented");
        exit(EXIT_FAILURE);
      }
      uint8_t ms_two = (num & GET_MS_2) >> (WORD_SIZE - 2);
      num = (num << 2) | ms_two; 
    }
    op2 = ((op2 | shift) << 8) | num;
  } else {
    /*  Operand2 as Rm{, <shift>}
     *  1. get Rm
     *  2. process shift
     */  
    // 1 : 
    char *rm_str = strtok(op_as_str, " ,"); 
    uint8_t rm = GET_REG_FROM_STR(rm_str);
    // 2 : 
    char *shift_str = strtok(NULL, "");
    uint8_t shift = parse_shift(shift_str);

    op2 = ((op2 | shift) << 4) | rm;  

  }
  dp->operand2 = op2;
}

uint32_t encode_dp(decoded_instr_t *dec) {
  uint32_t binary_instr = 0;
  data_processing_t *dp = &(dec->dp);
  // bit 31 - 28 : condition code
  binary_instr |= dec->cond;
  // bit 27 - 26 : 00
  // bit 25 : immediate flag
  binary_instr <<= 3;
  binary_instr |= dp->imm;
  // bit 24 - 21 : opcode
  binary_instr <<= 4;
  binary_instr |= dp->opcode;
  // bit 20 : set condition codes flag
  binary_instr <<= 1;
  binary_instr |= dp->set_cc;
  // bit 19 - 16 : Rn
  binary_instr <<= 4;
  binary_instr |= dp->rn;
  // bit 15 - 12 : Rd
  binary_instr <<= 4;
  binary_instr |= dp->rd;
  // bit 11 - 0 : operand 2
  binary_instr <<= 12;
  binary_instr |= dp->operand2;
} 