#pragma once
#include <string>

enum Reg {
  ZERO = 0x0,
  PC = 0x1,
  IRA = 0x2,
  CSR = 0x3,
  RA = 0x4,
  SP = 0x5,
  FP = 0x6,
  COUT = 0x7,
  S0 = 0x8,
  S1 = 0x9,
  S2 = 0xA,
  S3 = 0xB,
  T0 = 0xC,
  T1 = 0xD,
  T2 = 0xE,
  T3 = 0xF
};

uint16_t str_to_reg(std::string reg_name);

enum OP {
  CALC = 0x0,
  CALCI = 0x1,
  LOAD = 0x2,
  STORE = 0x3,
  JMPIF = 0x4,
  FCALL = 0x5
};

bool is_calc(std::string op);
bool is_calci(std::string op);
uint16_t str_to_opc(std::string op_s);

enum Func {
  ADD = 0x0,
  SUB = 0x1,
  AND = 0x2,
  OR = 0x3,
  XOR = 0x4,
  NOT = 0x5,
  SRS = 0x6,
  SRU = 0x7,
  SL = 0x8,
  EQ = 0x9,
  LTS = 0xA,
  LTU = 0xB,
  LNOT = 0xC,
  LCAST = 0xD
};

uint16_t str_to_func(std::string op_s);

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

extern std::string mnemonic;
extern std::string pseudo_mnemonic;

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
