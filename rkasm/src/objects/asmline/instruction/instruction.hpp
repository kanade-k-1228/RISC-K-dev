#pragma once
#include "../imm/imm.hpp"
#include "arch/arch.hpp"

class Instruction {
  uint16_t address;
  std::string mnemonic;
  std::vector<std::string> operand;
  std::string func, rs1, rs2, rd;
  Imm imm;
  Format format;
  uint32_t bin;
public:
  Instruction(){};
  Instruction(const uint16_t address, const std::vector<std::string> str);
  void genBin();
  uint32_t getBin() { return bin; }
  void resoluteLabel(std::vector<Label> labels);
  friend std::string printPretty(Instruction&);
  friend std::string printFormat(Instruction&);
  static bool match(std::vector<std::string> splitd);
};
