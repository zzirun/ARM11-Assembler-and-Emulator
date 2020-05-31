#include "../types.h"
#include "../utils.h"
#include "datafunctions.h"

void multiply(machine_state* ms){
    multiply_t *instr = &ms->instr_to_execute.mul;
    word_t result;

    // Multiplying Rm and Rs
    result = ms->regs.gpr[instr->rm] * ms->regs.gpr[instr->rs];

    // If A bit is set then accumulate with Rn
    if(instr->accum){
        result += ms->regs.gpr[instr->rn];
    }

    // Write to destination register
    ms->regs.gpr[instr->rd] = result;

    //Update flags if S bit is set
    if(instr->set_cc){
        // extract the top 4 bits of the cpsr
        word_t new_cpsr = (ms->regs.cpsr) >> 28 ;
        //set Z using mask
        if(result == 0){
            new_cpsr |= Z ;
        }
        //set N flag to 31 bit of result using mask
        if(result >> 31){
            new_cpsr |= N;
        }

        //Update cpsr to newcpsr
        ms->regs.cpsr = new_cpsr << 28;

    }

}
