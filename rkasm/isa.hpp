#pragma once
#include <string>

const std::string mnemonic_calc = "add|sub|and|or|xor|not|srs|sru|sl|eq|lts|ltu|lcast";
const std::string mnemonic_calci = "addi|subi|andi|ori|xori|eqi|ltsi|ltui";
const std::string mnemonic_calif = "if|jump|call|ret|iret";
const std::string mnemonic = "add|sub|and|or|xor|not|srs|sru|sl|eq|lts|ltu|lcast|"
                             "addi|subi|andi|ori|xori|eqi|ltsi|ltui|"
                             "mov|loadi|load|store|if|jump|call|ret|iret";

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
