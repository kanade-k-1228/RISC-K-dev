#pragma once
#include <string>
#include <tuple>
#include <vector>

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

const std::vector<std::string> mnemonics_calc = {"add", "not", "sl", "lrot", "and", "xor", "or", "sub", "eq", "neq", "ltu", "lts", "sru", "srs", "rrot"};
const std::vector<std::string> mnemonics_calci = {"addi", "andi", "xori", "ori", "subi", "eqi", "neqi", "ltui", "ltsi"};
const std::vector<std::string> mnemonics_load = {"load", "pop"};
const std::vector<std::string> mnemonics_store = {"store", "push"};
const std::vector<std::string> mnemonics_ctrl = {"if", "ifr", "jump", "jumpr", "call", "ret", "iret"};

struct Format {
  std::string mnemonic;
  std::vector<std::string> operand;
  uint8_t opc;
  uint8_t func;
  std::array<int, 5> binary;
};

const std::vector<Format> isa = {
    {"add", {"rd", "rs1", "rs2"}, OPCode::calc, ALUCode::ADD, {OPCode::calc, 0, 0, 0, ALUCode::ADD}},
    {"not", {"rd", "rs1", "rs2"}, OPCode::calc, ALUCode::NOT, {OPCode::calc, 0, 0, 0, ALUCode::NOT}},
    {"sl", {"rd", "rs1", "rs2"}, OPCode::calc, ALUCode::SL, {OPCode::calc, 0, 0, 0, ALUCode::SL}},
    {"lrot", {"rd", "rs1", "rs2"}, OPCode::calc, ALUCode::LROT, {OPCode::calc, 0, 0, 0, ALUCode::LROT}},
    {"and", {"rd", "rs1", "rs2"}, OPCode::calc, ALUCode::AND, {OPCode::calc, 0, 0, 0, ALUCode::AND}},
    {"xor", {"rd", "rs1", "rs2"}, OPCode::calc, ALUCode::XOR, {OPCode::calc, 0, 0, 0, ALUCode::XOR}},
    {"or", {"rd", "rs1", "rs2"}, OPCode::calc, ALUCode::OR, {OPCode::calc, 0, 0, 0, ALUCode::OR}},
    {"sub", {"rd", "rs1", "rs2"}, OPCode::calc, ALUCode::SUB, {OPCode::calc, 0, 0, 0, ALUCode::SUB}},
    {"eq", {"rd", "rs1", "rs2"}, OPCode::calc, ALUCode::EQ, {OPCode::calc, 0, 0, 0, ALUCode::EQ}},
    {"neq", {"rd", "rs1", "rs2"}, OPCode::calc, ALUCode::NEQ, {OPCode::calc, 0, 0, 0, ALUCode::NEQ}},
    {"ltu", {"rd", "rs1", "rs2"}, OPCode::calc, ALUCode::LTU, {OPCode::calc, 0, 0, 0, ALUCode::LTU}},
    {"lts", {"rd", "rs1", "rs2"}, OPCode::calc, ALUCode::LTS, {OPCode::calc, 0, 0, 0, ALUCode::LTS}},
    {"sru", {"rd", "rs1", "rs2"}, OPCode::calc, ALUCode::SRU, {OPCode::calc, 0, 0, 0, ALUCode::SRU}},
    {"srs", {"rd", "rs1", "rs2"}, OPCode::calc, ALUCode::SRS, {OPCode::calc, 0, 0, 0, ALUCode::SRS}},
    {"rrot", {"rd", "rs1", "rs2"}, OPCode::calc, ALUCode::RROT, {OPCode::calc, 0, 0, 0, ALUCode::RROT}},

    {"nop", {}, OPCode::calc, ALUCode::ADD, {OPCode::calc, 0, 0, 0, ALUCode::ADD}},
    {"mov", {"rd", "rs1"}, OPCode::calc, ALUCode::ADD, {OPCode::calc, 0, 0, 0, ALUCode::ADD}},

    {"addi", {"rd", "rs1", "imm"}, OPCode::calci, ALUCode::ADD, {OPCode::calci, 0, ALUCode::ADD, 0, 0}},
    {"andi", {"rd", "rs1", "imm"}, OPCode::calci, ALUCode::AND, {OPCode::calci, 0, ALUCode::AND, 0, 0}},
    {"xori", {"rd", "rs1", "imm"}, OPCode::calci, ALUCode::XOR, {OPCode::calci, 0, ALUCode::XOR, 0, 0}},
    {"ori", {"rd", "rs1", "imm"}, OPCode::calci, ALUCode::OR, {OPCode::calci, 0, ALUCode::OR, 0, 0}},
    {"subi", {"rd", "rs1", "imm"}, OPCode::calci, ALUCode::SUB, {OPCode::calci, 0, ALUCode::SUB, 0, 0}},
    {"eqi", {"rd", "rs1", "imm"}, OPCode::calci, ALUCode::EQ, {OPCode::calci, 0, ALUCode::EQ, 0, 0}},
    {"neqi", {"rd", "rs1", "imm"}, OPCode::calci, ALUCode::NEQ, {OPCode::calci, 0, ALUCode::NEQ, 0, 0}},
    {"ltui", {"rd", "rs1", "imm"}, OPCode::calci, ALUCode::LTU, {OPCode::calci, 0, ALUCode::LTU, 0, 0}},
    {"ltsi", {"rd", "rs1", "imm"}, OPCode::calci, ALUCode::LTS, {OPCode::calci, 0, ALUCode::LTS, 0, 0}},

    {"loadi", {"rd", "imm"}, OPCode::calci, ALUCode::ADD, {OPCode::calci, 0, ALUCode::ADD, 0, 0}},

    {"load", {"rd", "rs1", "imm"}, OPCode::load, ALUCode::ADD, {OPCode::load, 0, ALUCode::ADD, 0, 0}},
    {"pop", {"rd", "rs1", "imm"}, OPCode::load, ALUCode::ADD, {OPCode::load, 0, ALUCode::ADD, 0, 1}},

    {"store", {"rd", "rs1", "imm"}, OPCode::store, ALUCode::ADD, {OPCode::store, 0, ALUCode::ADD, 0, 0}},
    {"push", {"rd", "rs1", "imm"}, OPCode::store, ALUCode::ADD, {OPCode::store, 0, ALUCode::ADD, 0, 0}},

    {"if", {"rs1", "imm"}, OPCode::ctrl, ALUCode::ADD, {OPCode::ctrl, Reg::zero, 0, Reg::zero, 0}},
    {"ifr", {"rs1 ", " imm "}, OPCode::ctrl, ALUCode::ADD, {OPCode::ctrl, Reg::pc, Reg::zero, Reg::zero, 0}},
    {"jump", {"imm"}, OPCode::ctrl, ALUCode::ADD, {OPCode::ctrl, Reg::zero, Reg::zero, Reg::zero, 0}},
    {"jumpr", {"imm"}, OPCode::ctrl, ALUCode::ADD, {OPCode::ctrl, Reg::pc, Reg::zero, Reg::zero, 0}},
    {"call", {"imm"}, OPCode::ctrl, ALUCode::ADD, {OPCode::ctrl, Reg::zero, Reg::zero, Reg::ra, 0}},
    {"ret", {}, OPCode::ctrl, ALUCode::ADD, {OPCode::ctrl, Reg::ra, Reg::zero, Reg::zero, 0}},
    {"iret", {}, OPCode::ctrl, ALUCode::ADD, {OPCode::ctrl, Reg::ira, Reg::zero, Reg::zero, 0}}};

const Format& getFormat(std::string mnemonic);
bool is_mnemonic(std::string str);
