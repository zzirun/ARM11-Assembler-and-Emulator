CC = gcc
CFLAGS = -g -Wall -pedantic
FILES = emulate.o utils.o decode.o execute.o instructions/branch.o instructions/data_processing.o instructions/multiply.o instructions/single_data_transfer.o instructions/operandExtraction.o instructions/halt.o

#gcc -g -Wall -pedantic emulate.c utils.c decode.c execute.c instructions/branch.c instructions/data_processing.c instructions/multiply.c instructions/single_data_transfer.c instructions/operandExtraction.c instructions/halt.c -o emulate -lm

emulate: $(FILES)
	$(CC) $(CFLAGS) $(FILES) -o emulate -lm

emulate.o: emulate.c utils.h types.h decode.h 

utils.o: utils.c utils.h types.h

decode.o: decode.c utils.h types.h decode.h instructions/instructions.h

execute.o: execute.c utils.h types.h instructions/instructions.h instructions/datafunctions.h

instructions/branch.o: instructions/branch.c instructions/instructions.h utils.h types.h instructions/datafunctions.h

instructions/dataprocessing.o: instructions/dataprocessing.c instructions/instructions.h utils.h types.h instructions/datafunctions.h instructions/operandExtraction.h

instructions/multiply.o: instructions/multiply.c instructions/instructions.h utils.h types.h instructions/datafunctions.h

instructions/single_data_transfer.o: instructions/single_data_transfer.c instructions/instructions.h utils.h types.h instructions/datafunctions.h instructions/operandExtraction.h

instructions/operandExtraction.o: instructions/operandExtraction.c utils.h types.h instructions/operandExtraction.h

instructions/halt.o: instructions/halt.c utils.h types.h instructions/datafunctions.h

clean:
	rm -rf *.o instructions/*.o emulate
