#pragma once
#include <string>

const std::string mnemonic_calc = "add|sub|and|or|xor|not|srs|sru|ls|eq|lts|ltu|lcast|mov";
const std::string mnemonic_calci = "addi|subi|andi|ori|xori|eqi|ltsi|ltui|loadi";
const std::string mnemonic = "add|sub|and|or|xor|not|srs|sru|sl|eq|lts|ltu|lcast|mov|"
                             "addi|subi|andi|ori|xori|eqi|ltsi|ltui|loadi|"
                             "load|store|if|jump|call|ret|iret";
const std::string mnemonic_pseudo = "loadi|call|ret|iret";

namespace OPCode {
const uint8_t calc = 0b0000;
const uint8_t calci = 0b0001;
const uint8_t load = 0b0011;
const uint8_t store = 0b0111;
const uint8_t calif = 0b1111;
}  // namespace OPCode

namespace ALUCode {
const uint8_t ADD = 0x0;
const uint8_t SUB = 0x1;
const uint8_t AND = 0x2;
const uint8_t OR = 0x3;
const uint8_t XOR = 0x4;
const uint8_t NOT = 0x5;
const uint8_t SRS = 0x6;
const uint8_t SRU = 0x7;
const uint8_t SL = 0x8;
const uint8_t EQ = 0x9;
const uint8_t LTS = 0xA;
const uint8_t LTU = 0xB;
const uint8_t LCAST = 0xC;
}  // namespace ALUCode

namespace REG {
const uint8_t zero = 0x0;
const uint8_t pc = 0x1;
const uint8_t ira = 0x2;
const uint8_t csr = 0x3;
const uint8_t ra = 0x4;
const uint8_t sp = 0x5;
const uint8_t fp = 0x6;
const uint8_t cout = 0x7;
const uint8_t s0 = 0x8;
const uint8_t s1 = 0x9;
const uint8_t s2 = 0xA;
const uint8_t s3 = 0xB;
const uint8_t t0 = 0xC;
const uint8_t t1 = 0xD;
const uint8_t t2 = 0xE;
const uint8_t t3 = 0xF;
}  // namespace REG

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
