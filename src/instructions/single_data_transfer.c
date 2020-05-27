//
//
#include "instructions.h"
#include "types.h"
#include "utils.c"
#include "operandExtraction.h"

void data_processing(machine_state *machine, registers *r, memory mem) {
    decoded_inst inst = machine->instrToExecute;
    uint32_t address;
    uint32_t offset;

    if (inst.I) {
        // immediate flag is set, offset interpreted as
        // shifted register
        bool carry;
        offset = regExtract(inst.operand_offset, r, &carry);
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
        address = r->gpr[inst.Rn];
        r->gpr[inst.Rn] = address + offset;
    } else {
        // P flag is not set, offset is added/subtracted
        // to the base register after transferring
        address = r->gpr[inst.Rn] + offset;
    }

    if (inst.L) {
        // L bit is set, word is loaded from memory
        r->gpr[inst.Rd] = load_word(address, mem);
    } else {
        // L bit is not set, word is stored into memory
        store_word(address, mem, r->gpr[inst.Rd]);
    }




}


