CC = gcc
CFLAGS = -g -Wall -pedantic
FILES = emulate.o utils.o decode.o execute.o instructions/branch.o instructions/data_processing.o instructions/multiply.o instructions/single_data_transfer.o instructions/operandExtraction.o 

#gcc -g -Wall -pedantic emulate.c utils.c decode.c execute.c instructions/branch.c instructions/data_processing.c instructions/multiply.c instructions/single_data_transfer.c instructions/operandExtraction.c instructions/halt.c -o emulate -lm

emulate: $(FILES)
	$(CC) $(CFLAGS) $(FILES) -o emulate 

emulate.o: emulate.c utils.h types.h 

utils.o: utils.c utils.h types.h

decode.o: decode.c utils.h types.h

execute.o: execute.c utils.h types.h instructions/datafunctions.h

instructions/branch.o: instructions/branch.c utils.h types.h instructions/datafunctions.h

instructions/dataprocessing.o: instructions/dataprocessing.c utils.h types.h instructions/datafunctions.h instructions/operandExtraction.h

instructions/multiply.o: instructions/multiply.c utils.h types.h instructions/datafunctions.h

instructions/single_data_transfer.o: instructions/single_data_transfer.c utils.h types.h instructions/datafunctions.h instructions/operandExtraction.h

instructions/operandExtraction.o: instructions/operandExtraction.c utils.h types.h instructions/operandExtraction.h

clean:
	rm -rf *.o instructions/*.o emulate

.PHONY: clean
