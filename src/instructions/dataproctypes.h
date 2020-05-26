// dp instr opcode representations
enum opcode {
  AND = 0,  // 0000 : a & b
  EOR = 1,  // 0001 : a ^ b
  SUB = 2,  // 0010 : a - b
  RSB = 3,  // 0011 : b - a
  ADD = 4,  // 0100 : a + b
  TST = 8,  // 1000 : AND, no write
  TEQ = 9,  // 1001 : EOR, no write
  CMP = 10, // 1010 : SUB, no write
  ORR = 12, // 1100 : a | b
  MOV = 13  // 1101 : b
};
