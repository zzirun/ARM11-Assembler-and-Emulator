#include "assemble_instruction_utils.h"

char *trim(char *str) {
  if (str) {
    while (TO_REMOVE(*str)) {
      str++;
    }
    char *end = str + strlen(str) - 1;
    while (end > str && TO_REMOVE(*end)) {
      end--;
    }
    end[1] = '\0';
  }
  return str;
}

bool contains(char *s, char c) {
  for (; *s; s++) {
    if (c == *s) {
      return 1;
    }
  }
  return 0;
}

int num_of_operands(char *str) {
  int result = 1;
  for (int i = 0; str[i] != '\0'; i++) {
    if (str[i] == ',') {
      result++;
    }
  }
  return result;
}

/*  Parses numerical constant <#/=expression> string (base 10/16), 
 *  * Returns magnitude of integer
 *  * Modifies output parameter sign if needed 
 *    (+, 1), (-, 0), default : +
 */
uint32_t parse_numerical_expr(char *num_str, bool *sign) {
  assert(*num_str == '#' || *num_str == '=');
  // Skip over leading character '#'/'='
  num_str++; 
  // Check for possible sign
  if (*num_str == '+' || *num_str == '-') {
    if (sign) {
      *sign = (*num_str == '-') ? 0 : 1;
    }
    num_str++; 
  }
  // Find magnitude, check for prefix "0x", parse on base
  if (num_str[0] == '0' && num_str[1] == 'x') {
    return parse_hex(num_str);  // hexadecimal
  }
  else {
    return atoi(num_str); // decimal
  }
}

/* Parses a hexadecimal string into a integer, unsigned */
uint32_t parse_hex(char *hex_str) {
  assert(hex_str[0] == '0' && hex_str[1] == 'x');
  // Skip over "0x"
  hex_str += 2; 
  // Find number by looping over each character
  uint32_t num = 0;
  for (int i = 0; i < strlen(hex_str); i++) {
    num *= HEXADECIMAL_BASE;
    char c = hex_str[i];
    if (IN_RANGE(c, 'A', 'F')) {
      num += c - 'A' + A_AS_DECIMAL;
    } else if (IN_RANGE(c, 'a', 'f')) {
      num += c - 'a' + A_AS_DECIMAL;
    } else if (IN_RANGE(c, '0', '9')) {
      num += c - '0';
    } else {
      fprintf(stderr, "Invalid Character in Instruction");
      exit(EXIT_FAILURE);
    }
  }
  return num;
}

/*  Translates string to shift type  */
shift_t get_shift_type(char *shift_type_str) {
  if (!strcmp("lsl", shift_type_str)) {
    return LSL_S;
  }
  if (!strcmp("lsr", shift_type_str)) {
    return LSR_S;
  }
  if (!strcmp("asr", shift_type_str)) {
    return ASR_S;
  }
  if (!strcmp("ror", shift_type_str)) {
    return ROR_S;
  }
  fprintf(stderr, "Invalid Shift Type");
  exit(EXIT_FAILURE);
}

/*  Parses a (possibly null) shift string 
 *  * form: <shiftname> <#expression>/<register>
 *  Returns encoding of shift as in DP/SDT
 */
uint8_t parse_shift(char *shift_str) {
  /* No shift */
  if (!shift_str) {
    return 0;
  }
  /* Has shift */
  // Get shift type <shiftname>
  char *shift_field = strtok(shift_str, " ");
  uint8_t shift_type = get_shift_type(shift_field);
  // Get shift amount
  // And move shift amount to correct bit position
  shift_field = trim(strtok(NULL, ""));
  bool shift_by_reg;
  uint8_t shift_amount;
  if (*shift_field == '#') {
    // shift by constant amount: <#expression>
    shift_by_reg = 0;
    shift_amount = parse_numerical_expr(shift_field, NULL);
    shift_amount <<= 3;
  }
  else if (*shift_field == 'r') {
    // shift by bottom byte of register: <register>
    shift_by_reg = 1;
    shift_amount = GET_REG_FROM_STR(shift_field);
    shift_amount <<= 4;
  }
  else {
    fprintf(stderr, "Invalid Shift Operand");
    exit(EXIT_FAILURE);
  }
  // Build binary representation of shift
  return shift_amount | (shift_type << 1) | shift_by_reg;
}

// TO DO : ADD ERRORS FOR INVALID CASES
/*  Helper for DP and SDT assembler 
 *  Assigns 
 *  * immediate flag
 *  * operand 2 for DP / offset for SDT
 *  * only SDT: u flag
 */
void get_op_from_str(char *op_as_str, decoded_instr_t *instr) {
  assert(instr->type == DATA_PROC || instr->type == DATA_TRANS);
  if (!op_as_str) {
    return;
  }
  // Set immediate and operand2/offset to modify
  bool *imm = instr->type == DATA_PROC ? &instr->dp.imm : &instr->sdt.imm;
  uint16_t *operand = instr->type == DATA_PROC ? &instr->dp.operand2 : &instr->sdt.offset;

  uint16_t op = 0;
  if (*op_as_str == '#') {
    /**  Operand as <#expression> **/
    /* CASE DP */
    if (instr->type == DATA_PROC) {
      *imm = 1;
      // Get number from expression
      uint32_t num = parse_numerical_expr(op_as_str, NULL);
      // Convert to representation if possible: 
      // (8-bit unsigned immediate + 4-bit ROR shift amount)
      // Reverse ROR until fits 8-bit imm or exceed max shamt
      uint8_t shift = 0;
      while (num > MAX_DP_IMM) {
        if (++shift > MAX_DP_SHAMT) {
          fprintf(stderr, "Operand Cannot Be Represented");
          exit(EXIT_FAILURE);
        }
        uint8_t ms_two = (num & GET_MS_2) >> (WORD_SIZE - 2);
        num = (num << 2) | ms_two;
      }
      // Build binary representation 
      op = ((op | shift) << 8) | num;
    }

    /* CASE SDT : Operand as number */
    if (instr->type == DATA_TRANS) {
      *imm = 0;
      op =  parse_numerical_expr(op_as_str, &instr->sdt.u);
    }

  } else if (*op_as_str == 'r' || *op_as_str == '-' || *op_as_str == '+') {
    /** Operand as shifted register **/
    /* Set conditions unique to DP and SDT */
    if (instr->type == DATA_PROC) {
      // check only register type (no sign) for DP
      if (*op_as_str != 'r') {
        fprintf(stderr, "Invalid Operand");
        exit(EXIT_FAILURE);
      }
      *imm = 0;
    }
    if (instr->type == DATA_TRANS) {
      *imm = 1;
      // check to set U flag and remove sign character
      instr->sdt.u = *op_as_str == '-' ? 0 : 1;
      op_as_str = strtok(op_as_str, "+-");
    }
    /* Operand as Rm{, <shift>} (same for DP and SDT) */
    // Get Rm (reg to be shifted) 
    char *rm_str = strtok(op_as_str, " ,");
    uint8_t rm = GET_REG_FROM_STR(rm_str);
    // Process shift
    char *shift_str = trim(strtok(NULL, ""));
    uint8_t shift = parse_shift(shift_str);
    // Build binary representation
    op = ((op | shift) << 4) | rm;
    
  }
  else {
    fprintf(stderr, "Invalid Operand");
    exit(EXIT_FAILURE);
  }

  // Set Operand
  *operand = op;
}

