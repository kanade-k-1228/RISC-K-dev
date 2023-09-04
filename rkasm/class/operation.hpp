#pragma once
#include "imm.hpp"
#include "label.hpp"

struct Operation {
  uint16_t address;
  std::string mnemonic;
  std::vector<std::string> operand;
  std::string func, rs1, rs2, rd;
  Imm imm;
  Operation(){};
  Operation(const uint16_t address, const std::vector<std::string> str);
  uint32_t getBin();
  std::string print();
};