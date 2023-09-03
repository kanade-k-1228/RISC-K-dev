#pragma once
#include <string>
#include <vector>

const std::vector<std::string> mnemonics_calc = {"add", "not", "sl", "lrot", "and", "xor", "or", "sub", "eq", "neq", "ltu", "lts", "sru", "srs", "rrot"};
const std::vector<std::string> mnemonics_calci = {"addi", "andi", "xori", "ori", "subi", "eqi", "neqi", "ltui", "ltsi"};
const std::vector<std::string> mnemonics_load = {"load", "pop"};
const std::vector<std::string> mnemonics_store = {"store", "push"};
const std::vector<std::string> mnemonics_ctrl = {"if", "ifr", "jump", "jumpr", "call", "ret", "iret"};

const std::vector<std::string> mnemonics = {
    "nop",
    "mov",

    "add",
    "not",
    "sl",
    "lrot",
    "and",
    "xor",
    "or",
    "sub",
    "eq",
    "neq",
    "ltu",
    "lts",
    "sru",
    "srs",
    "rrot",

    "loadi",

    "addi",
    "andi",
    "xori",
    "ori",
    "subi",
    "eqi",
    "neqi",
    "ltui",
    "ltsi",

    "load",
    "pop",

    "store",
    "push",

    "if",
    "ifr",
    "jump",
    "jumpr",
    "call",
    "ret",
    "iret"};

namespace OPCode {
const uint8_t calc = 0b0000;
const uint8_t calci = 0b0001;
const uint8_t load = 0b0011;
const uint8_t store = 0b0111;
const uint8_t ctrl = 0b1111;
}  // namespace OPCode

namespace ALUCode {
const uint8_t ADD = 0x0;
const uint8_t NOT = 0x1;
const uint8_t SL = 0x2;
const uint8_t LROT = 0x3;
const uint8_t AND = 0x4;
const uint8_t XOR = 0x5;
const uint8_t OR = 0x6;
const uint8_t SUB = 0x7;
const uint8_t EQ = 0x8;
const uint8_t NEQ = 0x9;
const uint8_t LTU = 0xA;
const uint8_t LTS = 0xB;
const uint8_t SRU = 0xC;
const uint8_t SRS = 0xD;
const uint8_t RROT = 0xE;
}  // namespace ALUCode

namespace Reg {
const uint8_t zero = 0x0;
const uint8_t ira = 0x1;
const uint8_t pc = 0x2;
const uint8_t sp = 0x3;
const uint8_t ra = 0x4;
const uint8_t fp = 0x5;
const uint8_t a0 = 0x6;
const uint8_t a1 = 0x7;
const uint8_t t0 = 0x8;
const uint8_t t1 = 0x9;
const uint8_t t2 = 0xA;
const uint8_t t3 = 0xB;
const uint8_t s0 = 0xC;
const uint8_t s1 = 0xD;
const uint8_t s2 = 0xE;
const uint8_t s3 = 0xF;
}  // namespace Reg

namespace CSRBit {
const uint16_t IEN = 0b0000'0000'0001;
const uint16_t INTR = 0b0000'0000'0010;
const uint16_t INTR0 = 0b0000'0000'0100;
const uint16_t INTR1 = 0b0000'0000'1000;
const uint16_t INTR2 = 0b0000'0001'0000;
const uint16_t INTR3 = 0b0000'0010'0000;
const uint16_t SRCV = 0b0000'0100'0000;
const uint16_t SSEND = 0b0000'1000'0000;
const uint16_t CSTOP = 0b0001'0000'0000;
}  // namespace CSRBit

namespace Addr {
const uint16_t PC_INTR = 0x0001;
}
