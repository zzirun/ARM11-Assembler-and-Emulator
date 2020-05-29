#include <stdbool.h>
#include <stdio.h>
#include "types.h"
#include "instructions/instructions.h"
#include "instructions/datafunctions.h"
#include "utils.h"

// check instr cond codes with most significant 4 bits of CPSR (NZCV)
bool checkCond(machine_state *ms) {
  uint32_t cpsrMSFour = (ms->regs.CPSR) >> 28;
  switch (ms -> instrToExecute.cond) {
    case EQ:
      return cpsrMSFour & Z;
    case NE:
      return !(cpsrMSFour & Z);
    case GE:
      return ((cpsrMSFour & N) >> 3) == (cpsrMSFour & V);
    case LT:
      return ((cpsrMSFour & N) >> 3) != (cpsrMSFour & V);
    case GT:
      return !(cpsrMSFour & Z) && (((cpsrMSFour & N) >> 3) == (cpsrMSFour & V));
    case LE:
      return (cpsrMSFour & Z) || (cpsrMSFour & N) >> 3 != (cpsrMSFour & V);
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
    if (ms->instrToExecute.type == HALT) { 
      halt(ms);
    }
    if(checkCond(ms)) {
        switch(ms->instrToExecute.type){
            case DATA_PROC:
                dataProc(ms);
                break;
            case MULT:
                mult(ms);
                break;
            case DATA_TRANS:
                single_data_transfer(ms);
                break;
            case BRANCH:
                branch(ms);
                break;
            default:
                fprintf(stderr, "Invalid Instruction Type at Address: %x \n", ms->regs.PC -8);
                terminate(ms);
                // unsuccessful exit;
        }
    }
}
