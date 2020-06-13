CC = gcc
CFLAGS = -g -Wall -pedantic
FILES = emulate.o utils.o decode.o execute.o

#gcc -g -Wall -pedantic emulate.c utils.c decode.c execute.c -o emulate -lm

all: emulate assemble

emulate: $(FILES)
	$(CC) $(CFLAGS) $(FILES) -o emulate

emulate.o: emulate.c emulate.h utils.h

utils.o: utils.c utils.h emulate.h

decode.o: decode.c utils.h emulate.h

execute.o: execute.c utils.h emulate.h

assemble: assemble.o assembler/listfunctions.o assembler/mult_assembler.o assembler/assemble_instructions.o assembler/assemble_utils.o assembler/branch.o assembler/dp.o assembler/tokenizer.o #assembler/sdt.o

assemble.o: assemble.c assembler/assemble.h assembler/assemble_utils.h

assembler/listfunctions.o: assembler/listfunctions.c assembler/assembletypes.h

assembler/mult_assembler.o: assembler/mult_assembler.c assembler/assemble.h assembler/assemble_utils.h

assembler/assemble_instructions.o: assembler/assemble_instructions.c assembler/assemble_utils.h assembler/assemble.h

assembler/assemble_utils.o: assembler/assemble_utils.c assembler/assemble_utils.h assembler/assemble.h

assembler/branch.o: assembler/branch.c assembler/assemble.h assembler/assemble_utils.h

assembler/dp.o: assembler/dp.c assembler/assemble_utils.h assembler/assemble.h

assembler/tokenizer.o: assembler/tokenizer.c assembler/assemble.h assembler/assemble_utils.h

#assembler/sdt.o: assembler/sdt.c types.h assembler/assembletypes.h assembler/assemble_utils.h


clean:
	rm -rf *.o assembler/*.o assemble emulate

.PHONY: clean all
