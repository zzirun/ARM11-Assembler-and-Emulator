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

assemble: assemble.o assembler/symbol_table_utils.o assembler/program_utils.o assembler/mult_assembler.o assembler/assemble_instructions.o assembler/assemble_utils.o assembler/branch.o assembler/dp.o assembler/tokenizer.o assembler/sdt.o assembler/instruction_utils.o

assemble.o: assemble.c assembler/assemble.h assembler/instruction_utils.h

assembler/symbol_table_utils.o: assembler/symbol_table_utils.c assembler/assemble_types.h

assembler/program_utils.o: assembler/symbol_table_utils.c assembler/assemble_types.h

assembler/mult_assembler.o: assembler/mult_assembler.c assembler/assemble.h assembler/instruction_utils.h

assembler/assemble_instructions.o: assembler/assemble_instructions.c assembler/instruction_utils.h assembler/assemble.h

assembler/instruction_utils.o: assembler/instruction_utils.c assembler/instruction_utils.h assembler/assemble.h

assembler/assemble_utils.o: assembler/assemble_utils.c assembler/instruction_utils.h

assembler/branch.o: assembler/branch.c assembler/assemble.h assembler/instruction_utils.h

assembler/dp.o: assembler/dp.c assembler/instruction_utils.h assembler/assemble.h

assembler/tokenizer.o: assembler/tokenizer.c assembler/assemble.h assembler/instruction_utils.h

assembler/sdt.o: assembler/sdt.c types.h assembler/assemble_types.h assembler/instruction_utils.h


clean:
	rm -rf *.o assembler/*.o assemble emulate

.PHONY: clean all
