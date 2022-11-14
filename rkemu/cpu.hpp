#include <array>
#include <fstream>
#include <iostream>

class CPU {
public:
  std::array<uint16_t, 0xffff> mem;
  uint16_t pc = 0;
  std::array<uint32_t, 0xffff> rom;

  CPU();

  void load_rom(std::string);
  void dump();

  void step();

  void add(uint16_t, uint16_t, uint16_t);
  void addi(uint16_t, uint16_t, uint16_t);
  void sub(uint16_t, uint16_t, uint16_t);
  void subi(uint16_t, uint16_t, uint16_t);
  void land(uint16_t, uint16_t, uint16_t);
  void landi(uint16_t, uint16_t, uint16_t);
  void lor(uint16_t, uint16_t, uint16_t);
  void lori(uint16_t, uint16_t, uint16_t);
  void lnot(uint16_t, uint16_t, uint16_t);
  void load(uint16_t, uint16_t, uint16_t);
  void loadi(uint16_t, uint16_t);
  void store(uint16_t, uint16_t, uint16_t);
  void jump(uint16_t, uint16_t, uint16_t);
  void breq(uint16_t, uint16_t, uint16_t);
  void brlt(uint16_t, uint16_t, uint16_t);
};
