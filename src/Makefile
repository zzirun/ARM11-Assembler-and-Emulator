CC = gcc
CFLAGS = -g -Wall -pedantic

all: emulate assemble

emulate: emulate.o emulator/emulate_instruction_utils.o emulator/emulate_utils.o emulator/decode.o emulator/execute.o

emulate.o: emulate.c emulator/emulate.h

emulator/emulate_instruction_utils.o: emulator/emulate_instruction_utils.c emulator/emulate_instruction_utils.h

emulator/emulate_utils.o: emulator/emulate_utils.c emulator/emulate.h 

emulator/decode.o: emulator/decode.c emulator/emulate.h

emulator/execute.o: emulator/execute.c emulator/emulate.h

assemble: assemble.o assembler/symbol_table_utils.o assembler/program_utils.o assembler/assemble_instructions.o assembler/assemble_utils.o assembler/tokenizer.o assembler/assemble_instruction_utils.o

assemble.o: assemble.c assembler/assemble.h

assembler/symbol_table_utils.o: assembler/symbol_table_utils.c assembler/assemble_types.h

assembler/program_utils.o: assembler/program_utils.c assembler/assemble_types.h

assembler/instruction_utils.o: assembler/instruction_utils.c assembler/assemble_instruction_utils.h

assembler/assemble_instructions.o: assembler/assemble_instructions.c assembler/assemble.h

assembler/assemble_utils.o: assembler/assemble_utils.c assembler/assemble.h

assembler/tokenizer.o: assembler/tokenizer.c assembler/assemble.h 

clean:
	rm -rf *.o assembler/*.o assemble emulate

.PHONY: clean all
