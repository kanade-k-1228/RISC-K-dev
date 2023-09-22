#pragma once
#include "../rkarch/arch.hpp"
#include <array>
#include <fstream>
#include <string>

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
  bool irq_latch[4] = {false};
public:
  RAM ram;
  std::array<uint32_t, 0x10000> rom;
  std::string fname;

  CPU();

  void load_rom_file(std::string);

  void execute(uint32_t);
  void calc(uint8_t alu_func, uint16_t rd, uint16_t rs1, uint16_t rs2);
  void calci(uint8_t alu_func, uint16_t rd, uint16_t rs1, uint16_t imm);
  void load(uint16_t, uint16_t, uint16_t);
  void store(uint16_t, uint16_t, uint16_t);
  void ctrl(uint16_t, uint16_t, uint16_t, uint16_t);

  bool is_shutdowned() { return ram.get(CSR::power); }
  int serial();
  void interrupt(uint16_t irq_flag);
};
