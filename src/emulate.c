#include "utils.h"
#include "emulate.h"

int main(int argc, char **argv) {
    /*
     *  Machine state when turned on is zero
     *  15 registers initialised to zero
     *  Memory of ARM emulator has 64KB, initialised to zero
     */
    machine_state *ms = (machine_state *) calloc(sizeof(machine_state), 1);
    // check for successful memory allocation
    if (!ms) {
        fprintf(stderr, "Unsuccessful Memory Allocation");
        return EXIT_FAILURE;
    }

    // Load content of binary file into memory
    // argv[1] to skip space character
    bin_load(argv[1], ms);
    
    // PIPELINE - Execute, Decode, Fetch cycle
    /*
     *  Run this loop if we need to decode or
     *  execute an instruction that isn't HALT
     */
    while ((ms->ps != DECODED) || (ms->instr_to_execute.type != HALT)) {
        switch (ms->ps) {
            case (DECODED):
                execute(ms);
            case (FETCHED):
                decode(ms);
            case (EMPTY):
                // fetch
                ms->instr_fetched = load_word(ms->regs.pc, ms);
                // update PC
                ms->regs.pc += 4;
                // update processor state
                ms->ps = !ms->ps ? FETCHED : DECODED;
                break;
            default:
                fprintf(stderr, "Invalid Processor State");
                return EXIT_FAILURE;
        }
    }

    //HALT
    output(ms);

    free(ms);

    return EXIT_SUCCESS;
}

