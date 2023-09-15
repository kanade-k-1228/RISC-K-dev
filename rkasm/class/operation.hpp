#pragma once
#include "../../rkarch/arch.hpp"
#include "imm.hpp"
#include "label.hpp"

class Operation {
  uint16_t address;
  std::string mnemonic;
  std::vector<std::string> operand;
  std::string func, rs1, rs2, rd;
  Imm imm;
  Format format;
  uint32_t bin;
public:
  Operation(){};
  Operation(const uint16_t address, const std::vector<std::string> str);
  void genBin();
  uint32_t getBin() { return bin; }
  std::string print_pretty();
  std::string print_format();
  void resoluteLabel(std::vector<Label> labels);
};
