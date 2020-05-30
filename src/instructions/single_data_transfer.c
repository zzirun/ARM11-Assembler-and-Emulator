#include "instructions.h"
#include "../types.h"
#include "../utils.h"
#include "operandExtraction.h"
#include "datafunctions.h"

void single_data_transfer(machine_state *ms) {
    decoded_inst inst = ms->instrToExecute;
    uint32_t address;
    uint32_t offset;

    if (inst.imm) {
        // immediate flag is set, offset interpreted as
        // shifted register
        bool carry;
        offset = regExtract(inst.operand_offset, ms, &carry);
    } else {
        // immediate flag not set, offset interpreted as
        // an unsigned 12 bit immediate offset
        offset = inst.operand_offset;
    }

    if (!inst.u) {
        // U flag not set, offset is subtracted from base register
        offset = (~offset) + 1;
    }

    if (inst.p) {
        // P flag is set, offset is added/subtracted
        // to the base register before transferring the data
        address = ms->regs.gpr[inst.rn] + offset;
    } else {
        // P flag is not set, offset is added/subtracted
        // to the base register after transferring
        address = ms->regs.gpr[inst.rn];
        ms->regs.gpr[inst.rn] = address + offset;
    }

    if (inst.l) {
        // L bit is set, word is loaded from memory
        ms->regs.gpr[inst.rd] = load_word(address, ms);
    } else {
        // L bit is not set, word is stored into memory
        store_word(address, ms, ms->regs.gpr[inst.rd]);
    }




}
