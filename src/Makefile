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

assemble: assemble.o

assemble.o: assemble.c assembler/listfunctions.o

assembler/listfunctions.o: assembler/assembletypes.h assembler/listfunctions.c

clean:
	rm -rf *.o instructions/*.o assemble emulate bin/*

.PHONY: clean all
