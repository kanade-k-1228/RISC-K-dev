#pragma once
#include "format.hpp"
#include <vector>

namespace OPC {
const uint8_t calc = 0b0000;
const uint8_t calci = 0b0001;
const uint8_t load = 0b0011;
const uint8_t store = 0b0111;
const uint8_t ctrl = 0b1111;
}  // namespace OPC

namespace Func {
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
}  // namespace Func

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

const std::vector<Format> isa = {
    // Addi
    // Arithmetic
    {"add", {"rd", "rs1", "rs2"}, {OPC::calc, "rs1", "rs2", "rd", Func::ADD}},
    {"sub", {"rd", "rs1", "rs2"}, {OPC::calc, "rs1", "rs2", "rd", Func::SUB}},
    // Bit
    {"and", {"rd", "rs1", "rs2"}, {OPC::calc, "rs1", "rs2", "rd", Func::AND}},
    {"or", {"rd", "rs1", "rs2"}, {OPC::calc, "rs1", "rs2", "rd", Func::OR}},
    {"xor", {"rd", "rs1", "rs2"}, {OPC::calc, "rs1", "rs2", "rd", Func::XOR}},
    // Compare
    {"eq", {"rd", "rs1", "rs2"}, {OPC::calc, "rs1", "rs2", "rd", Func::EQ}},
    {"neq", {"rd", "rs1", "rs2"}, {OPC::calc, "rs1", "rs2", "rd", Func::NEQ}},
    {"ltu", {"rd", "rs1", "rs2"}, {OPC::calc, "rs1", "rs2", "rd", Func::LTU}},
    {"lts", {"rd", "rs1", "rs2"}, {OPC::calc, "rs1", "rs2", "rd", Func::LTS}},
    // Unarry
    {"not", {"rd", "rs1"}, {OPC::calc, "rs1", 0, "rd", Func::NOT}},
    {"sl", {"rd", "rs1"}, {OPC::calc, "rs1", 0, "rd", Func::SL}},
    {"lrot", {"rd", "rs1"}, {OPC::calc, "rs1", 0, "rd", Func::LROT}},
    {"sru", {"rd", "rs1"}, {OPC::calc, "rs1", 0, "rd", Func::SRU}},
    {"srs", {"rd", "rs1"}, {OPC::calc, "rs1", 0, "rd", Func::SRS}},
    {"rrot", {"rd", "rs1"}, {OPC::calc, "rs1", 0, "rd", Func::RROT}},
    // Psudo
    {"nop", {}, {OPC::calc, 0, 0, 0, Func::ADD}},
    {"mov", {"rd", "rs1"}, {OPC::calc, "rs1", 0, "rd", Func::ADD}},

    // Calci
    // Arithmetic
    {"addi", {"rd", "rs1", "imm"}, {OPC::calci, "rs1", Func::ADD, "rd", "imm"}},
    {"subi", {"rd", "rs1", "imm"}, {OPC::calci, "rs1", Func::SUB, "rd", "imm"}},
    // Bit
    {"andi", {"rd", "rs1", "imm"}, {OPC::calci, "rs1", Func::AND, "rd", "imm"}},
    {"ori", {"rd", "rs1", "imm"}, {OPC::calci, "rs1", Func::OR, "rd", "imm"}},
    {"xori", {"rd", "rs1", "imm"}, {OPC::calci, "rs1", Func::XOR, "rd", "imm"}},
    // Compare
    {"eqi", {"rd", "rs1", "imm"}, {OPC::calci, "rs1", Func::EQ, "rd", "imm"}},
    {"neqi", {"rd", "rs1", "imm"}, {OPC::calci, "rs1", Func::NEQ, "rd", "imm"}},
    {"ltui", {"rd", "rs1", "imm"}, {OPC::calci, "rs1", Func::LTU, "rd", "imm"}},
    {"ltsi", {"rd", "rs1", "imm"}, {OPC::calci, "rs1", Func::LTS, "rd", "imm"}},
    // Psudo
    {"loadi", {"rd", "imm"}, {OPC::calci, 0, Func::ADD, "rd", "imm"}},

    // Load
    {"load", {"rd", "rs1", "imm"}, {OPC::load, "rs1", Func::ADD, "rd", "imm"}},
    {"pop", {"rd"}, {OPC::load, Reg::sp, Func::ADD, "rd", 1}},

    // Store
    {"store", {"rs2", "rs1", "imm"}, {OPC::store, "rs1", "rs2", Func::ADD, "imm"}},
    {"push", {"rs2"}, {OPC::store, Reg::sp, "rs2", Func::ADD, 0}},

    // Control
    {"if", {"rs1", "imm"}, {OPC::ctrl, "rs1", Reg::zero, Reg::zero, "imm"}},
    {"ifr", {"rs1", "imm"}, {OPC::ctrl, "rs1", Reg::pc, Reg::zero, "imm"}},
    {"jump", {"imm"}, {OPC::ctrl, Reg::zero, Reg::zero, Reg::zero, "imm"}},
    {"jumpr", {"imm"}, {OPC::ctrl, Reg::pc, Reg::zero, Reg::zero, "imm"}},
    {"call", {"imm"}, {OPC::ctrl, Reg::zero, Reg::zero, Reg::ra, "imm"}},
    {"ret", {}, {OPC::ctrl, Reg::ra, Reg::zero, Reg::zero, 0}},
    {"iret", {}, {OPC::ctrl, Reg::ira, Reg::zero, Reg::zero, 0}}};
