#pragma once
#include "../rkisa/isa.hpp"
#include <array>
#include <fstream>
#include <string>

struct OPDecoder {
  uint16_t opc, func, rs1, rs2, rd, imm;
  OPDecoder(uint32_t code) {
    opc = (code) & 0x000F;
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
    // Read Only Address
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
  void pop(uint16_t reg) { mem.at(reg) = mem.at(++mem.at(Reg::sp)); }
  void push(uint16_t reg) { mem.at(mem.at(Reg::sp)--) = mem.at(reg); }
};

class CPU {
public:
  RAM ram;
  std::array<uint32_t, 0x10000> rom;
  std::string fname;
  bool irq_latch[4] = {false};

  CPU();

  void load_rom_file(std::string);

  bool is_shutdowned() { return ram.get(CSR::power); }
  int serial();

  void external_irq(int);
  void catch_interrupt();
  void jump_interrupt();

  void execute(uint32_t);
  void calc(uint8_t alu_func, uint16_t rd, uint16_t rs1, uint16_t rs2);
  void calci(uint8_t alu_func, uint16_t rd, uint16_t rs1, uint16_t imm);
  void load(uint16_t, uint16_t, uint16_t);
  void store(uint16_t, uint16_t, uint16_t);
  void ctrl(uint16_t, uint16_t, uint16_t, uint16_t);
};
