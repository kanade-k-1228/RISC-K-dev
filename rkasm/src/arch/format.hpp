#pragma once
#include <array>
#include <string>
#include <variant>
#include <vector>

using BitField = std::variant<int, std::string>;

struct Format {
  std::string mnemonic;
  std::vector<std::string> operand;
  std::array<BitField, 5> binary;
};
