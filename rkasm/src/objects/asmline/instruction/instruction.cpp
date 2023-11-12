#include "instruction.hpp"
#include "arch/arch.hpp"
#include "utils/utils.hpp"
#include <regex>
#include <sstream>

Instruction::Instruction(const uint16_t address, const std::vector<std::string> str)
    : address(address), mnemonic(str.at(0)), rs1("zero"), rs2("zero"), rd("zero") {

  format = get_operation(mnemonic).value();

  const std::vector<std::string> arg(str.begin() + 1, str.end());
  if(arg.size() != format.operand.size())
    throw new std::string("Required " + std::to_string(format.operand.size()) + " Operand");

  // Read arguments value
  for(size_t i = 0; i < format.operand.size(); ++i) {
    const std::string arg_type = format.operand.at(i);
    if(arg_type == "rd") rd = arg.at(i);
    if(arg_type == "rs1") rs1 = arg.at(i);
    if(arg_type == "rs2") rs2 = arg.at(i);
    if(arg_type == "imm") imm = Imm(arg.at(i));
  }
}

void Instruction::genBin() {
  std::array<int, 5> slot = format.binary;
  for(auto arg_type : format.operand) {
    if(arg_type == "rs1") slot.at(1) = reg_stoi(rs1);
    if(arg_type == "rs2") slot.at(2) = reg_stoi(rs2);
    if(arg_type == "rd") slot.at(3) = reg_stoi(rd);
    if(arg_type == "imm") slot.at(4) = imm.getValue();
  }
  bin = (slot.at(0) & 0x0F)
        | ((slot.at(1) & 0x0F) << 4)
        | ((slot.at(2) & 0x0F) << 8)
        | ((slot.at(3) & 0x0F) << 12)
        | ((slot.at(4) & 0xFFFF) << 16);
}

Label& findLabel(std::vector<Label> vec, std::string name) {
  for(auto& label : vec)
    if(label.is(name)) return label;
  throw new std::string("Undefined label: " + name);
}

void Instruction::resoluteLabel(std::vector<Label> labels) {
  if(imm.isLabRef()) {
    Label labref = findLabel(labels, imm.getLabel());
    if(labref.isOpr()) {
      imm.resoluteAsOpr(labref.getValue());
    } else if(labref.isVar()) {
      imm.resoluteAsVar(labref.getValue());
    } else if(labref.isConst()) {
      imm.resoluteAsConst(labref.getValue());
    }
  }
}
