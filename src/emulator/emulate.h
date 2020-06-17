#ifndef ARM11_17_EMULATE
#define ARM11_17_EMULATE

#include "emulate_instruction_utils.h"

/* Binary loader (binary file to array) */
void bin_load(char *f, machine_state* ms);

/* Decodes instruction into instr type and operands */
void decode( machine_state* ms);

/* Execute
 * (1) checks instr condition code against CPSR flags 
 * (2) calls relevant execution function based on instr type 
 */
void execute(machine_state* ms);
// (1)
bool check_cond(machine_state *ms);
// (2)
void data_processing(machine_state *ms);
void multiply(machine_state *ms);
void single_data_transfer(machine_state *ms);
void branch(machine_state *ms);

/* Output at HALT - prints regs and non-zero memory */ 
void output(machine_state* ms);

#endif
