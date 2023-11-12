#pragma once
#include <array>
#include <string>
#include <vector>

struct InstructionFormat {
  std::string mnemonic;
  std::vector<std::string> operand;
  uint8_t opc;
  uint8_t func;
  std::array<int, 5> binary;
};
