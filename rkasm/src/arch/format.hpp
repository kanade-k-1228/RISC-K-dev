#pragma once
#include <array>
#include <string>
#include <vector>

struct Format {
  std::string mnemonic;
  std::vector<std::string> operand;
  std::array<int, 5> binary;
};
