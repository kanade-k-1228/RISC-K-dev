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

namespace CalcFunc {
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
}  // namespace CalcFunc

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

namespace Addr {
const uint16_t PC_INTR = 0x0001;
}

namespace CSR {
const uint16_t power = 0x0010;
const uint16_t irq_en = 0x0020;
const uint16_t irq_0 = 0x0030;
const uint16_t irq_1 = 0x0031;
const uint16_t irq_2 = 0x0032;
const uint16_t irq_3 = 0x0033;
}  // namespace CSR

namespace Serial {
const uint16_t tx = 0x0100;
const uint16_t rx = 0x0101;
}  // namespace Serial

struct Format {
  std::string mnemonic;
  std::vector<std::string> operand;
  uint8_t opc;
  uint8_t func;
  std::array<int, 5> binary;
};

const std::vector<Format> isa = {
    // Addi
    // Arithmetic
    {"add", {"rd", "rs1", "rs2"}, OPCode::calc, CalcFunc::ADD, {OPCode::calc, 0, 0, 0, CalcFunc::ADD}},
    {"sub", {"rd", "rs1", "rs2"}, OPCode::calc, CalcFunc::SUB, {OPCode::calc, 0, 0, 0, CalcFunc::SUB}},
    // Bit
    {"and", {"rd", "rs1", "rs2"}, OPCode::calc, CalcFunc::AND, {OPCode::calc, 0, 0, 0, CalcFunc::AND}},
    {"or", {"rd", "rs1", "rs2"}, OPCode::calc, CalcFunc::OR, {OPCode::calc, 0, 0, 0, CalcFunc::OR}},
    {"xor", {"rd", "rs1", "rs2"}, OPCode::calc, CalcFunc::XOR, {OPCode::calc, 0, 0, 0, CalcFunc::XOR}},
    // Compare
    {"eq", {"rd", "rs1", "rs2"}, OPCode::calc, CalcFunc::EQ, {OPCode::calc, 0, 0, 0, CalcFunc::EQ}},
    {"neq", {"rd", "rs1", "rs2"}, OPCode::calc, CalcFunc::NEQ, {OPCode::calc, 0, 0, 0, CalcFunc::NEQ}},
    {"ltu", {"rd", "rs1", "rs2"}, OPCode::calc, CalcFunc::LTU, {OPCode::calc, 0, 0, 0, CalcFunc::LTU}},
    {"lts", {"rd", "rs1", "rs2"}, OPCode::calc, CalcFunc::LTS, {OPCode::calc, 0, 0, 0, CalcFunc::LTS}},
    // Unarry
    {"not", {"rd", "rs1"}, OPCode::calc, CalcFunc::NOT, {OPCode::calc, 0, 0, 0, CalcFunc::NOT}},
    {"sl", {"rd", "rs1"}, OPCode::calc, CalcFunc::SL, {OPCode::calc, 0, 0, 0, CalcFunc::SL}},
    {"lrot", {"rd", "rs1"}, OPCode::calc, CalcFunc::LROT, {OPCode::calc, 0, 0, 0, CalcFunc::LROT}},
    {"sru", {"rd", "rs1"}, OPCode::calc, CalcFunc::SRU, {OPCode::calc, 0, 0, 0, CalcFunc::SRU}},
    {"srs", {"rd", "rs1"}, OPCode::calc, CalcFunc::SRS, {OPCode::calc, 0, 0, 0, CalcFunc::SRS}},
    {"rrot", {"rd", "rs1"}, OPCode::calc, CalcFunc::RROT, {OPCode::calc, 0, 0, 0, CalcFunc::RROT}},
    // Psudo
    {"nop", {}, OPCode::calc, CalcFunc::ADD, {OPCode::calc, 0, 0, 0, CalcFunc::ADD}},
    {"mov", {"rd", "rs1"}, OPCode::calc, CalcFunc::ADD, {OPCode::calc, 0, 0, 0, CalcFunc::ADD}},

    // Calci
    // Arithmetic
    {"addi", {"rd", "rs1", "imm"}, OPCode::calci, CalcFunc::ADD, {OPCode::calci, 0, CalcFunc::ADD, 0, 0}},
    {"subi", {"rd", "rs1", "imm"}, OPCode::calci, CalcFunc::SUB, {OPCode::calci, 0, CalcFunc::SUB, 0, 0}},
    // Bit
    {"andi", {"rd", "rs1", "imm"}, OPCode::calci, CalcFunc::AND, {OPCode::calci, 0, CalcFunc::AND, 0, 0}},
    {"ori", {"rd", "rs1", "imm"}, OPCode::calci, CalcFunc::OR, {OPCode::calci, 0, CalcFunc::OR, 0, 0}},
    {"xori", {"rd", "rs1", "imm"}, OPCode::calci, CalcFunc::XOR, {OPCode::calci, 0, CalcFunc::XOR, 0, 0}},
    // Compare
    {"eqi", {"rd", "rs1", "imm"}, OPCode::calci, CalcFunc::EQ, {OPCode::calci, 0, CalcFunc::EQ, 0, 0}},
    {"neqi", {"rd", "rs1", "imm"}, OPCode::calci, CalcFunc::NEQ, {OPCode::calci, 0, CalcFunc::NEQ, 0, 0}},
    {"ltui", {"rd", "rs1", "imm"}, OPCode::calci, CalcFunc::LTU, {OPCode::calci, 0, CalcFunc::LTU, 0, 0}},
    {"ltsi", {"rd", "rs1", "imm"}, OPCode::calci, CalcFunc::LTS, {OPCode::calci, 0, CalcFunc::LTS, 0, 0}},
    // Psudo
    {"loadi", {"rd", "imm"}, OPCode::calci, CalcFunc::ADD, {OPCode::calci, 0, CalcFunc::ADD, 0, 0}},

    // Load
    {"load", {"rd", "rs1", "imm"}, OPCode::load, CalcFunc::ADD, {OPCode::load, 0, CalcFunc::ADD, 0, 0}},
    {"pop", {"rd"}, OPCode::load, CalcFunc::ADD, {OPCode::load, Reg::sp, CalcFunc::ADD, 0, 1}},

    // Store
    {"store", {"rs2", "rs1", "imm"}, OPCode::store, CalcFunc::ADD, {OPCode::store, 0, CalcFunc::ADD, 0, 0}},
    {"push", {"rs2"}, OPCode::store, CalcFunc::ADD, {OPCode::store, 0, CalcFunc::ADD, 0, 0}},

    // Control
    {"if", {"rs1", "imm"}, OPCode::ctrl, CalcFunc::ADD, {OPCode::ctrl, Reg::zero, 0, Reg::zero, 0}},
    {"ifr", {"rs1 ", " imm "}, OPCode::ctrl, CalcFunc::ADD, {OPCode::ctrl, Reg::pc, Reg::zero, Reg::zero, 0}},
    {"jump", {"imm"}, OPCode::ctrl, CalcFunc::ADD, {OPCode::ctrl, Reg::zero, Reg::zero, Reg::zero, 0}},
    {"jumpr", {"imm"}, OPCode::ctrl, CalcFunc::ADD, {OPCode::ctrl, Reg::pc, Reg::zero, Reg::zero, 0}},
    {"call", {"imm"}, OPCode::ctrl, CalcFunc::ADD, {OPCode::ctrl, Reg::zero, Reg::zero, Reg::ra, 0}},
    {"ret", {}, OPCode::ctrl, CalcFunc::ADD, {OPCode::ctrl, Reg::ra, Reg::zero, Reg::zero, 0}},
    {"iret", {}, OPCode::ctrl, CalcFunc::ADD, {OPCode::ctrl, Reg::ira, Reg::zero, Reg::zero, 0}}};

const Format& getFormat(std::string mnemonic);
bool is_mnemonic(std::string str);

uint16_t alu(uint8_t code, uint16_t a, uint16_t b);

struct Decoder {
  uint8_t opc;
  uint8_t rs1, rs2, rd;
  uint16_t imm;
  uint8_t func;
  Format format;
  Decoder(uint32_t bin)
      : opc(bin & 0xf),
        rs1((bin >> 4) & 0xf),
        rs2((bin >> 8) & 0xf),
        rd((bin >> 12) & 0xf),
        imm((bin >> 16) & 0xffff),
        func(0) {
    if(opc == OPCode::calc) func = (bin >> 16) & 0xf;
    if(opc == OPCode::calci) func = (bin >> 8) & 0xf;
  }
};
