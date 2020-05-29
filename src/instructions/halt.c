#include <stdio.h>
#include <stdlib.h>
#include "datafunctions.h"
#include "../types.h"
#include "../utils.h"

void halt(machine_state* ms) {
  output(ms);
  free(ms);
  exit(EXIT_SUCCESS);
}
