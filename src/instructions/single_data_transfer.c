//
//
#include "instructions.h"
#include "../types.h"
#include "../utils.h"
#include "operandExtraction.h"
#include "datafunctions.h"

void single_data_transfer(machine_state *ms) {
    decoded_inst inst = ms->instrToExecute;
    uint32_t address;
    uint32_t offset;

    if (inst.I) {
        // immediate flag is set, offset interpreted as
        // shifted register
        bool carry;
        offset = regExtract(inst.operand_offset, &(ms->regs), &carry);
    } else {
        // immediate flag not set, offset interpreted as
        // an unsigned 12 bit immediate offset
        offset = inst.operand_offset;
    }

    if (!inst.U) {
        // U flag not set, offset is subtracted from base register
        offset = (~offset) + 1;
    }

    if (inst.P) {
        // P flag is set, offset is added/subtracted
        // to the base register before transferring the data
        address = ms->regs.gpr[inst.Rn];
        ms->regs.gpr[inst.Rn] = address + offset;
    } else {
        // P flag is not set, offset is added/subtracted
        // to the base register after transferring
        address = ms->regs.gpr[inst.Rn] + offset;
    }

    if (inst.L) {
        // L bit is set, word is loaded from memory
        ms->regs.gpr[inst.Rd] = load_word(address, ms->mem);
    } else {
        // L bit is not set, word is stored into memory
        store_word(address, ms->mem, ms->regs.gpr[inst.Rd]);
    }




}
