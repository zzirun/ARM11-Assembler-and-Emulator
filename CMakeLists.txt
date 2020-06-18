cmake_minimum_required(VERSION 3.16)
project(arm11_17 C)

set(CMAKE_C_STANDARD 11)

include_directories(Extension)
include_directories(src/assembler)
include_directories(src/emulator)

add_executable(arm11_17
        Extension/tests/test_loader.c
        Extension/list_utils.c
        Extension/login.c
        Extension/login.h
        Extension/main.c
        Extension/merchant.c
        Extension/merchant.h
        Extension/runtest.c
        Extension/types.h
        Extension/utils.c
        Extension/utils.h
        src/assembler/assemble.h
        src/assembler/assemble_instruction_utils.c
        src/assembler/assemble_instruction_utils.h
        src/assembler/assemble_instructions.c
        src/assembler/assemble_types.h
        src/assembler/assemble_utils.c
        src/assembler/program_utils.c
        src/assembler/symbol_table_utils.c
        src/assembler/tokenizer.c
        src/emulator/decode.c
        src/emulator/emulate.h
        src/emulator/emulate_instruction_utils.c
        src/emulator/emulate_instruction_utils.h
        src/emulator/emulate_types.h
        src/emulator/emulate_utils.c
        src/emulator/execute.c
        src/assemble.c
        src/emulate.c)
