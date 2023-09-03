#pragma once
#include "isa.hpp"
#include <array>
#include <fstream>
#include <string>

struct OPDecoder {
  uint16_t opc, func, rs1, rs2, rd, imm;
  OPDecoder(uint32_t code) {
    opc = (code)&0x000F;
    func = opc == OPCode::calc    ? (code >> 16) & 0x000F
           : opc == OPCode::calci ? (code >> 8) & 0x000F
                                  : 0;
    rs1 = (code >> 4) & 0x000F;
    rs2 = (code >> 8) & 0x000F;
    rd = (code >> 12) & 0x000F;
    imm = (code >> 16) & 0xFFFF;
  }
};

class RAM {
  std::array<uint16_t, 0x10000> mem;
public:
  RAM() : mem{0} {}
  bool set(uint16_t addr, uint16_t value) {
    // Read Only
    if(addr == Reg::zero || addr == Reg::pc || addr == Reg::ira) {
      return false;
    } else {
      mem.at(addr) = value;
      return true;
    }
  };
  uint16_t get(uint16_t addr) { return mem.at(addr); }
  void set_pc(uint16_t value) { mem.at(Reg::pc) = value; }
  void inc_pc() { mem.at(Reg::pc) += 1; }
  void set_ira() { mem.at(Reg::ira) = mem.at(Reg::pc); }
};

class CPU {
public:
  RAM ram;
  std::array<uint32_t, 0x10000> rom;
  std::string fname;
  bool intr_latch[4] = {false};

  CPU();

  void load_rom(std::string);

  bool cstop();
  int serial();

  void external_interrupt(int);
  void catch_interrupt();
  void jump_interrupt();

  void execute(uint32_t);

  void add(uint16_t, uint16_t, uint16_t);
  void addi(uint16_t, uint16_t, uint16_t);
  void sub(uint16_t, uint16_t, uint16_t);
  void subi(uint16_t, uint16_t, uint16_t);
  void land(uint16_t, uint16_t, uint16_t);
  void landi(uint16_t, uint16_t, uint16_t);
  void lor(uint16_t, uint16_t, uint16_t);
  void lori(uint16_t, uint16_t, uint16_t);
  void lxor(uint16_t, uint16_t, uint16_t);
  void lxori(uint16_t, uint16_t, uint16_t);
  void lnot(uint16_t, uint16_t);
  void srs(uint16_t, uint16_t);
  void sru(uint16_t, uint16_t);
  void sl(uint16_t, uint16_t);
  void eq(uint16_t, uint16_t, uint16_t);
  void eqi(uint16_t, uint16_t, uint16_t);
  void lts(uint16_t, uint16_t, uint16_t);
  void ltsi(uint16_t, uint16_t, uint16_t);
  void ltu(uint16_t, uint16_t, uint16_t);
  void ltui(uint16_t, uint16_t, uint16_t);
  void lcast(uint16_t, uint16_t);

  void load(uint16_t, uint16_t, uint16_t);
  void store(uint16_t, uint16_t, uint16_t);
  void ctrl(uint16_t, uint16_t, uint16_t, uint16_t);
};
