#pragma once
#include <string>

const std::string mnemonic_calc = "add|sub|and|or|xor|not|srs|sru|ls|eq|lts|ltu|lcast";
const std::string mnemonic_calci = "addi|subi|andi|ori|xori|eqi|ltsi|ltui";
const std::string mnemonic = "add|sub|and|or|xor|not|srs|sru|sl|eq|lts|ltu|lcast|"
                             "addi|subi|andi|ori|xori|eqi|ltsi|ltui|"
                             "load|store|jmpif|fctrl";
const std::string mnemonic_pseudo = "loadi|call|ret|iret";

namespace OPCode {
const uint8_t calc = 0b0000;
const uint8_t calci = 0b0001;
const uint8_t load = 0b0011;
const uint8_t store = 0b0111;
const uint8_t jmpif = 0b1111;
const uint8_t fctrl = 0b1111;
const uint8_t calif = 0b1111;
}  // namespace OPCode

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

// extern std::string mnemonic;
// extern std::string mnemonic_pseudo;

// struct OPDecoder {
//   uint16_t opc;
//   uint16_t func;
//   uint16_t rs1;
//   uint16_t rs2;
//   uint16_t rd;
//   uint16_t imm;
//   OPDecoder(uint32_t code);
// };

// struct OPEncoder {
//   uint32_t bin;
//   OPEncoder(uint16_t op, uint16_t func, uint16_t rs1, uint16_t rs2, uint16_t rd, uint16_t imm);
// };
