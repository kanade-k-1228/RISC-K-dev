#pragma once
#include <string>

enum Reg {
  ZERO = 0x00,
  RA = 0x01,
  SP = 0x02,
  FP = 0x03,
  CSR = 0x04,
  IRA = 0x05,
  COUT = 0x06,
  OS0 = 0x0C,
  OS1 = 0x0D,
  OS2 = 0x0E,
  OS3 = 0x0F,
  S0 = 0x10,
  S1 = 0x11,
  S2 = 0x12,
  S3 = 0x13,
  T0 = 0x20,
  T1 = 0x21,
  T2 = 0x22,
  T3 = 0x23,
  A0 = 0x30,
  A1 = 0x31,
  A2 = 0x32,
  A3 = 0x33,
};

enum OP {
  CALC = 0x0,
  CALCI = 0x1,
  LOAD = 0x2,
  LOADI = 0x3,
  STORE = 0x4,
  JUMP = 0x5,
  BREQ = 0x6,
  BRLT = 0x7,
};

enum Func {
  ADD = 0x0,
  SUB = 0x1,
  AND = 0x2,
  OR = 0x3,
  XOR = 0x4,
  NOT = 0x5,
  LROT = 0x06,
  RROT = 0x07,
};

enum CSRBit {
  IEN = 0b0000'0000'0001,
  INTR = 0b0000'0000'0010,
  INTR0 = 0b0000'0000'0100,
  INTR1 = 0b0000'0000'1000,
  INTR2 = 0b0000'0001'0000,
  INTR3 = 0b0000'0010'0000,
  SRCV = 0b0000'0100'0000,
  SSEND = 0b0000'1000'0000,
  CSTOP = 0b0001'0000'0000,
};

enum Addr {
  PC_INTR = 0x0001,
};

struct OPDecoder {
  uint16_t opc;
  uint16_t func;
  uint16_t rs1;
  uint16_t rs2;
  uint16_t rd;
  uint16_t imm;
  OPDecoder(uint32_t code);
};

struct OPEncoder {
  uint32_t bin;
  OPEncoder(uint16_t op, uint16_t func, uint16_t rs1, uint16_t rs2, uint16_t rd, uint16_t imm);
};
