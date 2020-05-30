#include "instructions.h"
#include "../types.h"
#include "../utils.h"
#include "datafunctions.h"

void mult(machine_state* ms){
    decoded_inst instr = ms->instrToExecute;
    uint32_t result;

    // Multiplying Rm and Rs
    result = ms->regs.gpr[instr.rm] * ms->regs.gpr[instr.rs];

    // If A bit is set then accumulate with Rn
    if(instr.accum){
        result += ms->regs.gpr[instr.rn];
    }

    // Write to destination register
    ms->regs.gpr[instr.rd] = result;

    //Update flags if S bit is set
    if(instr.set_cc){
        // extract the top 4 bits of the CPSR
        uint32_t newCPSR = (ms->regs.CPSR) >> 28 ;
        //set Z using mask
        if(result == 0){
            newCPSR |= Z ;
        }
        //set N flag to 31 bit of result using mask
        if(result >> 31){
            newCPSR |= N;
        }

        //Update CPSR to newCPSR
        ms->regs.CPSR = newCPSR << 28;

    }

}
