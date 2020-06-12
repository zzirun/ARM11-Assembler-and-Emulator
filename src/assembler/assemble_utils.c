#include "assemble_utils.h"
#include "assemble.h"

/* From assembly file: Builds label to address map + Loads non-label instruction strings */
void first_pass(char *file_path, symbol_table_t *s_t, instr_list_t *i_l) {
	char buffer[MAX_LINE_LENGTH] = {0};
	FILE* assembly_file = fopen(file_path, "r");
	if (!assembly_file) {
		perror("Failed to open source code");
		exit(EXIT_FAILURE);
	}
	uint16_t address = 0;
	while (fgets(&buffer[0], sizeof(buffer), assembly_file)) {
		bool is_label = false;
		for (int i = 0; buffer[i]; i++) {
			if (buffer[i] == ':') {
				buffer[i] = '\0'; //Removes ':' from the label
				is_label = true;
				break;
			}
		}
		if (is_label) {
			add_mapping(s_t, buffer, address);
		} else {
			add_instr(i_l, buffer);
		}
		address += 4;
	}
	fclose(assembly_file);
}

/* Writes 32 bits instruction into stream according to little endian format */
void binary_writer(instr_list_t *instructions, char *file_path) {
  FILE* binary_file = fopen(file_path, "wb");
  if (!binary_file) {
		perror("Failed to open binary file");
		exit(EXIT_FAILURE);
	}
	instr_t *curr = instructions->head->next;
  uint8_t inst_arr[NUMBER_OF_BYTES_PER_INST];
  for (; curr; curr = curr->next) { 
	  uint32_t instr = curr->binary_instr;
    for (int i = 0; i < NUMBER_OF_BYTES_PER_INST; i++) {
      inst_arr[i] = instr & GET_LS_8;
      instr >>= BYTE_SIZE;
    }
    fwrite(inst_arr, sizeof(uint8_t), NUMBER_OF_BYTES_PER_INST, binary_file);
  }
  fclose(binary_file);
}

/*  Parses numerical constant <#expression> string into an integer */
uint32_t parse_numerical_expr(char *num_str) {
  assert(*num_str == '#');
  num_str++; // skip over '#'
  uint32_t num = 0;
  // check for prefix "0x"
  if (num_str[0] == '0' && num_str[1] == 'x') {
    // hexadecimal
    num = parse_hex(num_str);
  } else {
    // decimal
    num = atoi(num_str);
  }
  return num;
}

/* Parses a hexadecimal string into an integer */
uint32_t parse_hex(char *hex_str) {
  assert(*hex_str == '0' && *hex_str == 'x');
  hex_str += 2; // skip over "0x"
  uint32_t num = 0;
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

/*  Translates string to shift type  */
shift_type get_shift_type(char *shift_type_str) {
  if (!strcmp("lsl", shift_type_str)) return LSL_S;
  if (!strcmp("lsr", shift_type_str)) return LSR_S;
  if (!strcmp("asr", shift_type_str)) return ASR_S;
  if (!strcmp("ror", shift_type_str)) return ROR_S;
  
  fprintf(stderr, "Invalid Shift Type");
  exit(EXIT_FAILURE);
}

/*  Parses a (possibly null) shift from string to 
 *  binary for bit 11 - 4 of DP instructions
 *  where operand2 is a shifted register
 */
uint8_t parse_shift(char *shift_str) {
  /* No shift */
  if (!shift_str) return 0;

  /* Has shift */
  // Get shift type
  char *shift_field = strtok(shift_str, " ");
  uint8_t shift_t = get_shift_type(shift_field);

  // Get shift amount 
  // + move shift amount to correct bit position 
  shift_field = strtok(NULL, "");
  bool shift_by_reg;
  uint8_t shift_amount;
  if (*shift_field == '#') {
    // shift by constant amount: <#expression>
    shift_by_reg = 0;
    shift_amount = parse_numerical_expr(shift_field);
    shift_amount <<= 3;

  } else if (*shift_field == 'r') {
    // shift by bottom byte of register: <register>
    shift_by_reg = 1;
    shift_amount = GET_REG_FROM_STR(shift_field);
    shift_amount <<= 4;

  } else {
    fprintf(stderr, "Invalid Shift Operand");
    exit(EXIT_FAILURE);
  }

  // Build binary representation of shift 
  return shift_amount | (shift_t << 1) | shift_by_reg;
}

// TO DO : ADD ERRORS FOR INVALID CASES
/*  Assigns immediate field and operand2 for data processing instructions
 *  using the string form of operand2
 */
void get_op_from_str(char *op_as_str, data_processing_t *dp) {
  uint16_t op2 = 0;
  if (*op_as_str == '#') {
    dp->imm = 1;
    /*  Operand2 as <#expression>
     *  1. get number to represent
     *  2. convert to representation if possible: 
     *     8-bit unsigned immediate + 4-bit ROR shift amount 
     *     max imm = 0xFF = 255 , max shamt = 0xF X 2 = 30
     */
    // 1 :
    uint32_t num = parse_numerical_expr(op_as_str); 
    // 2 : reverse ROR until fit into 8-bit or exceed max shamt
    uint8_t shift = 0;
    while (num > MAX_DP_IMM) {
      if (++shift > MAX_DP_SHAMT) {
        fprintf(stderr, "Operand Cannot Be Represented");
        exit(EXIT_FAILURE);
      }
      uint8_t ms_two = (num & GET_MS_2) >> (WORD_SIZE - 2);
      num = (num << 2) | ms_two; 
    }
    op2 = ((op2 | shift) << 8) | num;
    
  } else if (*op_as_str == 'r') {
    dp->imm = 0;
    /*  Operand2 as Rm{, <shift>}
     *  1. get Rm (reg to be shifted)
     *  2. process shift
     */  
    // 1 : 
    char *rm_str = strtok(op_as_str, " ,"); 
    uint8_t rm = GET_REG_FROM_STR(rm_str);
    // 2 : 
    char *shift_str = strtok(NULL, "");
    uint8_t shift = parse_shift(shift_str);
    op2 = ((op2 | shift) << 4) | rm;  

  } else {
    fprintf(stderr, "Invalid Operand");
    exit(EXIT_FAILURE);
  }

  // Set Operand2
  dp->operand2 = op2;
}
