#include <assert.h>
#include "../login.c"

int main(int argc, char **argv){
    assert(argc == 2);
    FILE *f = fopen(argv[1], "r");
    login(f);
    return 0;
}

