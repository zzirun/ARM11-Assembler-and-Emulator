#include <stdbool.h>
#include "types.h"
#include "instructions.h"

// check instr cond codes with most significant 4 bits of CPSR (NZCV)
bool checkCond(machine_state *ms) {
  uint32_t cpsrMSFour = (regs -> CPSR) >> 28
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
