#include "instructions.h"
#include "../types.h"
#include "../utils.h"
#include "datafunctions.h"

void mult(machine_state* ms){
    decoded_inst instr = ms->instrToExecute;
    uint32_t result;

    // Multiplying Rm and Rs
    result = ms->regs.gpr[instr.Rm] * ms->regs.gpr[instr.Rs];

    // If A bit is set then accumulate with Rn
    if(instr.A == 1){
        result += ms->regs.gpr[instr.Rn];
    }

    // Write to destination register
    ms->regs.gpr[instr.Rd] = result;

    //Update flags if S bit is set
    if(instr.S == 1){
        // extract the top 4 bits of the CPSR
        uint32_t newCPSR = (ms->regs.CPSR) >> 28 ;
        //set Z using mask
        if(result == 0){
            newCPSR |= Z ;
        }
        //set n flag to 31 bit of result using mask
        if(result >> 31){
            newCPSR |= N;
        }

        //Update CPSR to newCPSR
        ms->regs.CPSR = newCPSR << 28;

    }

}
