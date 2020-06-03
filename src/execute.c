#include <stdbool.h>
#include <stdio.h>
#include "types.h"
#include "data_functions.h"
#include "utils.h"

// check instr cond codes with most significant 4 bits of CPSR (NZCV)
bool check_cond(machine_state *ms) {
  word_t cpsr_ms4 = (ms->regs.cpsr) >> 28;
  switch (ms -> instr_to_execute.cond) {
    case EQ:
      return cpsr_ms4 & Z;
    case NE:
      return !(cpsr_ms4 & Z);
    case GE:
      return ((cpsr_ms4 & N) >> 3) == (cpsr_ms4 & V);
    case LT:
      return ((cpsr_ms4 & N) >> 3) != (cpsr_ms4 & V);
    case GT:
      return !(cpsr_ms4 & Z) && (((cpsr_ms4 & N) >> 3) == (cpsr_ms4 & V));
    case LE:
      return (cpsr_ms4 & Z) || (cpsr_ms4 & N) >> 3 != (cpsr_ms4 & V);
    case AL:
      return true;
    default:
      fprintf(stderr, "Invalid Instruction Condition Code\n");
      terminate(ms);
      return false;
  }
}

/**Executes the current instruction to execute of the machine state if condition
 * is met
 */

void execute(machine_state* ms) {
    /* Not in switch cases as the checkCond for
       HALT returns false according to the CPSR
       flags, hence treated as special case */
    if(check_cond(ms)) {
        switch(ms->instr_to_execute.type){
            case DATA_PROC:
                data_processing(ms);
                break;
            case MULT:
                multiply(ms);
                break;
            case DATA_TRANS:
                single_data_transfer(ms);
                break;
            case BRANCH:
                branch(ms);
                break;
            default:
                fprintf(stderr, "Invalid Instruction Type at Address: %x \n", ms->regs.pc - 8);
                terminate(ms);
                // unsuccessful exit;
        }
    }
}
