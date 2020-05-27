


// logical left shift
uint32_t lsl(uint32_t op, uint8_t shift, uint8_t *carry) {
  *carry = extractBits(op, 32 - shift, 32 - shift); // last discarded bit
  return op << shift;
}

// logical right shift
uint32_t lsr(uint32_t op, uint8_t shift, uint8_t *carry) {
  *carry = extractBits(op, shift - 1, shift - 1); // last discarded bit
  return op >> shift;
}
// arithmetic right shift
uint32_t asr(uint32_t op, uint8_t shift, uint8_t *carry) {
  *carry = extractBits(op, shift - 1, shift - 1); // last discarded bit
  uint32_t mask = 0;
  if (extractBits(op, 31, 31)) {
    mask = (pow(2, shift) - 1) << (32 - shift);
  }
  return (op >> shift) | mask;
}

// rotate right
uint32_t ror(uint32_t op, uint8_t shift, uint8_t *carry) {
  *carry = extractBits(op, shift - 1, shift - 1); // last rotated bit
  uint32_t mask = extractBits(op, 0, shift - 1) << (32 - shift);
  return (op >> shift) | mask;
}
