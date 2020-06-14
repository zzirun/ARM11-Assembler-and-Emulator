cmake_minimum_required(VERSION 3.16)
project(arm11_17 C)

set(CMAKE_C_STANDARD 11)

include_directories(src)
include_directories(src/assembler)

add_executable(arm11_17
        Extension/login.c
        src/assembler/assemble.h
        src/assembler/assemble_instructions.c
        src/assembler/assemble_utils.c
        src/assembler/assemble_utils.h
        src/assembler/assembletypes.h
        src/assembler/branch.c
        src/assembler/dp.c
        src/assembler/listfunctions.c
        src/assembler/mult_assembler.c
        src/assembler/sdt.c
        src/assembler/tokenizer.c
        src/assemble.c
        src/decode.c
        src/emulate.c
        src/emulate.h
        src/execute.c
        src/types.h
        src/utils.c
        src/utils.h)
