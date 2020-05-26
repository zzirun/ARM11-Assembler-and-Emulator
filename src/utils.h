#ifndef UTILS_H
#define UTILS_H

uint32_t extractBits(uint32_t target, int start, int end);
void printBits(uint32_t x);
uint32_t buildInstruction(uint8_t* ptr);
uint32_t buildNonZeroValue(uint8_t* ptr);

#endif
