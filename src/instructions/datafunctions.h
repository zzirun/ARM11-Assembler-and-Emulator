#include "../types.h"

#ifndef ARM11_17_FUNCTIONS
#define ARM11_17_FUNCTIONS

// execution for diff instr types
void dataProc(machine_state *ms);
void mult(machine_state *ms);
void single_data_transfer(machine_state *ms);
void branch(machine_state *ms);
void halt(machine_state *ms);

#endif
