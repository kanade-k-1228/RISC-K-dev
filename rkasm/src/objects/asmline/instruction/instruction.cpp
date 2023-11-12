#include "instruction.hpp"
#include "arch/arch.hpp"
#include "utils/utils.hpp"
#include <regex>
#include <sstream>

Instruction::Instruction(const uint16_t address, const std::vector<std::string> splited)
    : address(address), mnemonic(splited.at(0)), operand(splited.begin() + 1, splited.end()),
      rs1("zero"), rs2("zero"), rd("zero") {

  format = get_operation(mnemonic).value();

  if(operand.size() != format.operand.size())
    throw new std::string("Required " + std::to_string(format.operand.size()) + " Operand");

  // Read arguments value
  for(size_t i = 0; i < format.operand.size(); ++i) {
    const std::string arg_type = format.operand.at(i);
    if(arg_type == "rd") rd = operand.at(i);
    if(arg_type == "rs1") rs1 = operand.at(i);
    if(arg_type == "rs2") rs2 = operand.at(i);
    if(arg_type == "imm") {
      imm = Imm(operand.at(i));
      immWIP = operand.at(i);
    }
  }
}

void Instruction::genBin() {
  auto slot = format.binary;
  for(auto arg_type : format.operand) {
    if(arg_type == "rs1") slot.at(1) = reg_stoi(rs1);
    if(arg_type == "rs2") slot.at(2) = reg_stoi(rs2);
    if(arg_type == "rd") slot.at(3) = reg_stoi(rd);
    if(arg_type == "imm") slot.at(4) = imm.getValue();
  }
  bin = (std::get<int>(slot.at(0)) & 0x0F)
        | ((std::get<int>(slot.at(1)) & 0x0F) << 4)
        | ((std::get<int>(slot.at(2)) & 0x0F) << 8)
        | ((std::get<int>(slot.at(3)) & 0x0F) << 12)
        | ((std::get<int>(slot.at(4)) & 0xFFFF) << 16);
}


void Instruction::resolveLabel(std::vector<Label> labels) {
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

bool Instruction::match(std::vector<std::string> splited) {
  auto s = splited.at(0);
  return is_mnemonic(s);
}
