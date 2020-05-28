#include <stdbool.h>
#include <stdio.h>
#include "types.h"
#include "instructions.h"
#include "branch.c"
#include "data_processing.c"
#include "multiply.c"
#include "single_data_transfer.c"

// check instr cond codes with most significant 4 bits of CPSR (NZCV)
bool checkCond(machine_state *ms, registers* regs) {
  uint32_t cpsrMSFour = (regs -> CPSR) >> 28;
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
  }
}

/**Executes the current instruction to execute of the machine state if condition
 * is met
 */

void execute(machine_state* ms, registers* regs, memory* mem){
    if(checkCond(ms, regs)){
        switch(ms->instrToExecute.type){
            case DATA_PROC:
                dataProc(ms,regs);
                break;
            case MULT:
                mult(ms,regs);
                break;
            case DATA_TRANS:
                single_data_transfer(ms,regs, *mem);
                break;
            case BRANCH:
                branch(&ms->instrToExecute, regs);
                break;
            default:
                fprintf(stderr, "Invalid Instruction Type at Address: %x \n", ms->instrFetched -8);
                terminate(ms, regs, mem);
                // unsuccessful exit;
        }
    }
}


