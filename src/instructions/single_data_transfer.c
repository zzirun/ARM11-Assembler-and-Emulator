//
//
#include "instructions.h"
#include "types.h"

void data_processing(machine_state *machine) {
    decoded_inst inst = machine->instrToExecute;
    uint32_t address;
    uint32_t offset;


    if (inst.I) {
        // immediate flag is set, offset interpreted as
        // shifted register
    } else {
        // immediate flag not set, offset interpreted as
        // an unsigned 12 bit immediate offset
        offset = inst.operand_offset;
    }

    if (inst.P) {
        // P flag is set, offset is added/subtracted
        // to the base register before transferring the data
    } else {
        // P flag is not set, offset is added/subtracted
        // to the base register after transferring

    }

    if (!inst.U) {
        // U flag not set, offset is subtracted from base register
        offset = ~offset;
    }

    if (inst.L) {
        // L bit is set, word is loaded from memory
    } else {
        // L bit is not set, word is stored into memory
    }







}


