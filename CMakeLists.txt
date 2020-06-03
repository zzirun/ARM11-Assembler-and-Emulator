cmake_minimum_required(VERSION 3.16)
project(arm11_17 C)

set(CMAKE_C_STANDARD 11)

include_directories(src)
include_directories(src/instructions)

add_executable(arm11_17
        src/assemble.c
        src/decode.c
        src/emulate.c
        src/execute.c
        src/types.h
        src/utils.c
        src/utils.h src/instructions/data_functions.c)
