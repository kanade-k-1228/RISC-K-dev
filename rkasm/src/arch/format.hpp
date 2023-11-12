#pragma once
#include <array>
#include <string>
#include <variant>
#include <vector>

using BinFormat = std::variant<int, std::string>;

struct Format {
  std::string mnemonic;
  std::vector<std::string> operand;
  std::array<BinFormat, 5> binary;
};
