#pragma once
#include "../label/label.hpp"
#include "arch/arch.hpp"
#include <map>

enum class ArgType { Reg,
                     Imm,
                     Lab };

class Instruction {
  uint16_t address;
  std::string mnemonic;
  std::vector<std::string> operand;

  std::vector<std::pair<std::string, std::string>> argMap;
  std::vector<std::tuple<std::string, std::string, int, ArgType, std::optional<Label>>> argResolved;

  Format format;
  uint32_t bin;
public:
  Instruction(){};
  Instruction(const uint16_t address, const std::vector<std::string> splited);

  void resolveArgs(std::vector<Label> labels);
  void genBin();

  uint32_t getBin() { return bin; }

  friend std::string printPretty(Instruction&);
  friend std::string printFormat(Instruction&);
  static bool match(std::vector<std::string> splitd);
};
