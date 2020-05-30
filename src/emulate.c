#include <stdlib.h>
#include <stdio.h>
#include "utils.h"
#include "types.h"

int main(int argc, char **argv) {
    /*
    Machine state when turned on is zero
    15 registers initialised to zero
    Memory of ARM emulator has 64KB, initialised to zero
    */
    machine_state *ms = (machine_state *) calloc(sizeof(machine_state), 1);
    // check for successful memory allocation
    if (!ms) {
        fprintf(stderr, "Unsuccessful Memory Allocation");
        return EXIT_FAILURE;
    }

    //Opens file, argv[1] to skip space character
    FILE *fp;
    if (!(fp = fopen(argv[1], "rb"))) {
        perror("Cannot Open File");
        terminate(ms);
    }
    //Loads content of the file into memory
    binLoad(fp, ms->mem);
    fclose(fp);

    //PIPELINE
    /*
      Run this loop if we need to decode or
      execute an instruction that isn't HALT
    */
    while ((ms->ps != DECODED) || (ms->instrToExecute.type != HALT)) {
        switch (ms->ps) {
            case (DECODED):
                execute(ms);
            case (FETCHED):
                decode(ms);
            case (EMPTY):
                ms->instrFetched = load_word(ms->regs.PC, ms);
                ms->regs.PC += 4;
                // update_processor_state(ms);
                ms->ps = !ms->ps ? FETCHED : DECODED;
                // default case?
        }
    }

    //HALT
    output(ms);

    free(ms);

    return EXIT_SUCCESS;
}
