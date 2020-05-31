#include "../types.h"
#include "../utils.h"
#include "operandExtraction.h"
#include "datafunctions.h"

void single_data_transfer(machine_state *ms) {
    single_data_transfer_t *instr = &ms->instr_to_execute.sdt;
    word_t address;
    word_t offset;

    if (instr->imm) {
        // immediate flag is set, offset interpreted as
        // shifted register
        bool carry;
        offset = reg_extract(instr->offset, ms, &carry);
    } else {
        // immediate flag not set, offset interpreted as
        // an unsigned 12 bit immediate offset
        offset = instr->offset;
    }

    if (!instr->u) {
        // U flag not set, offset is subtracted from base register
        offset = (~offset) + 1;
    }

    if (instr->p) {
        // P flag is set, offset is added/subtracted
        // to the base register before transferring the data
        address = ms->regs.gpr[instr->rn] + offset;
    } else {
        // P flag is not set, offset is added/subtracted
        // to the base register after transferring
        address = ms->regs.gpr[instr->rn];
        ms->regs.gpr[instr->rn] = address + offset;
    }

    if (instr->l) {
        // L bit is set, word is loaded from memory
        ms->regs.gpr[instr->rd] = load_word(address, ms);
    } else {
        // L bit is not set, word is stored into memory
        store_word(address, ms, ms->regs.gpr[instr->rd]);
    }




}
