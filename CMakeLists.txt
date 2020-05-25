cmake_minimum_required(VERSION 3.16)
project(arm11_17 C)

set(CMAKE_C_STANDARD 11)

include_directories(src)
include_directories(src/instructions)

add_executable(arm11_17
        src/instructions/branch.c
        src/instructions/data_processing.c
        src/instructions/halt.c
        src/instructions/instructions.h
        src/instructions/multiply.c
        src/instructions/single_data_transfer.c
        src/assemble.c
        src/bin_loader.c
        src/decode.c
        src/emulate.c
        src/types.h)
