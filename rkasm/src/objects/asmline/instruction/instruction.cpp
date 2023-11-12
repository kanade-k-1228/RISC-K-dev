#include "instruction.hpp"
#include "arch/arch.hpp"
#include "utils/utils.hpp"
#include <regex>
#include <sstream>

Instruction::Instruction(const uint16_t address, const std::vector<std::string> splited)
    : address(address), mnemonic(splited.at(0)), operand(splited.begin() + 1, splited.end()) {

  format = get_operation(mnemonic).value();

  if(operand.size() != format.operand.size()) {
    throw new std::string("Required " + std::to_string(format.operand.size()) + " Operand");
  }

  // Anotate arguments
  for(size_t i = 0; i < format.operand.size(); ++i) {
    const std::string arg_key = format.operand.at(i);
    argMap.emplace_back(arg_key, operand.at(i));
  }
}

void Instruction::resolveArgs(std::vector<Label> labels) {
  for(auto [key, str] : argMap) {
    if(key.front() == 'r') {
      argResolved.emplace_back(key, str, reg_stoi(str), ArgType::Reg, std::nullopt);
    }
    if(key.front() == 'i') {
      auto result = stoi_opt(str);
      if(result.has_value()) {
        argResolved.emplace_back(key, str, result.value(), ArgType::Imm, std::nullopt);
      } else {
        auto label = findLabel(labels, str);
        argResolved.emplace_back(key, str, label.getValue(), ArgType::Lab, std::optional<Label>(label));
      }
    }
  }
}

void Instruction::genBin() {
  std::vector<int> bin_sliced;
  for(auto s : format.binary) {
    if(std::holds_alternative<std::string>(s)) {
      auto key = std::get<std::string>(s);
      auto result = std::find_if(argResolved.begin(), argResolved.end(), [key](auto i) { return std::get<0>(i) == key; });
      bin_sliced.push_back(std::get<2>(*result));
    }
    if(std::holds_alternative<int>(s)) {
      auto val = std::get<int>(s);
      bin_sliced.push_back(val);
    }
  }
  bin = (bin_sliced.at(0) & 0x0F)
        | ((bin_sliced.at(1) & 0x0F) << 4)
        | ((bin_sliced.at(2) & 0x0F) << 8)
        | ((bin_sliced.at(3) & 0x0F) << 12)
        | ((bin_sliced.at(4) & 0xFFFF) << 16);
}

bool Instruction::match(std::vector<std::string> splited) {
  auto s = splited.at(0);
  return is_mnemonic(s);
}
