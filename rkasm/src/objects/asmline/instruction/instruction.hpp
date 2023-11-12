#pragma once
#include "../imm/imm.hpp"
#include "arch/arch.hpp"

using ImmWIP = std::variant<std::monostate, std::string, int, Label&>;

class Instruction {
  uint16_t address;
  std::string mnemonic;
  std::vector<std::string> operand;
  std::string rs1, rs2, rd;
  Imm imm;
  ImmWIP immWIP;
  Format format;
  uint32_t bin;
public:
  Instruction(){};
  Instruction(const uint16_t address, const std::vector<std::string> splited);

  void resolveLabel(std::vector<Label> labels);

  void genBin();
  uint32_t getBin() { return bin; }

  friend std::string printPretty(Instruction&);
  friend std::string printFormat(Instruction&);
  static bool match(std::vector<std::string> splitd);
};
