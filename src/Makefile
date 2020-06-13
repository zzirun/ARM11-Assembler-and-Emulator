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

assemble: assemble.o assembler/listfunctions.o assembler/mult_assembler.o

assemble.o: assemble.c assembler/assembletypes.h

assembler/listfunctions.o: assembler/listfunctions.c assembler/assembletypes.h

assembler/mult_assembler.o: assembler/mult_assembler.c assembler/assembletypes.h

clean:
	rm -rf *.o instructions/*.o assembler/*.o assemble emulate

.PHONY: clean all
