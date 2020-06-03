CC = gcc
CFLAGS = -g -Wall -pedantic
FILES = emulate.o utils.o decode.o execute.o instructions/data_functions.o instructions/operand_extraction.o

#gcc -g -Wall -pedantic emulate.c utils.c decode.c execute.c instructions/data_functions.c instructions/operandExtraction.c -o emulate -lm

emulate: $(FILES)
	$(CC) $(CFLAGS) $(FILES) -o emulate 

emulate.o: emulate.c utils.h types.h 

utils.o: utils.c utils.h types.h

decode.o: decode.c utils.h types.h

execute.o: execute.c utils.h types.h instructions/data_functions.h

instructions/data_functions.o: instructions/data_functions.c utils.h types.h instructions/data_functions.h instructions/operand_extraction.h

instructions/operand_extraction.o: instructions/operand_extraction.c utils.h types.h instructions/operand_extraction.h

clean:
	rm -rf *.o instructions/*.o assemble emulate

.PHONY: clean
