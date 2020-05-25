cmake_minimum_required(VERSION 3.16)
project(arm11_17 C)

set(CMAKE_C_STANDARD 11)

add_executable(arm11_17
        src/assemble.c
        src/bin_loader.c
        src/emulate.c src/instructions/instructions.h src/instructions/data_processing.c src/decode.c)
