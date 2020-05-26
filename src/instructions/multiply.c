
#include "instructions.h"
#include "types.h"
#include "utils.h"

void mult(machine_state* ms, registers* regs){
    decoded_inst instr = ms->instrToExecute;
    uint32_t result;

    // Multiplying Rm and Rs
    result = instr.Rm * instr.Rs;

    // If A bit is set then accumulate with Rn
    if(instr.A == 1){
        result += instr.Rn;
    }

    // Write to destination register
    regs->gpr[instr.Rd] = result;

    //Update flags if S bit is set
    if(instr.S == 1){
        // extract the top 4 bits of the CPSR
        uint32_t newCPSR = (regs->CPSR) >> 28 ;
        //set Z using mask
        if(result == 0){
            newCPSR |= 0x4 ;
        }
        //set n flag to 31 bit of result using mask
        if(result >> 31){
            newCPSR |= 0x8;
        }

        //Update CPSR to newCPSR
        regs->CPSR = newCPSR << 28;

    }

}
