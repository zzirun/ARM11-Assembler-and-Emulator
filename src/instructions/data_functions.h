#include "../types.h"

#ifndef ARM11_17_FUNCTIONS
#define ARM11_17_FUNCTIONS

// execution for diff instr types
void data_processing(machine_state *ms);
void multiply(machine_state *ms);
void single_data_transfer(machine_state *ms);
void branch(machine_state *ms);

#endif
