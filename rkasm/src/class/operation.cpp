#include "operation.hpp"
#include "arch/arch.hpp"
#include "utils/utils.hpp"
#include <regex>
#include <sstream>

uint16_t reg_stoi(std::string name) {
  if(name == "zero") return Reg::zero;
  if(name == "pc") return Reg::pc;
  if(name == "ira") return Reg::ira;
  if(name == "sp") return Reg::sp;
  if(name == "ra") return Reg::ra;
  if(name == "fp") return Reg::fp;
  if(name == "a0") return Reg::a0;
  if(name == "a1") return Reg::a1;
  if(name == "t0") return Reg::t0;
  if(name == "t1") return Reg::t1;
  if(name == "t2") return Reg::t2;
  if(name == "t3") return Reg::t3;
  if(name == "s0") return Reg::s0;
  if(name == "s1") return Reg::s1;
  if(name == "s2") return Reg::s2;
  if(name == "s3") return Reg::s3;
  throw new std::string("Invalid Registor Name: " + name);
  return 0;
}

Operation::Operation(const uint16_t address, const std::vector<std::string> str)
    : address(address), mnemonic(str.at(0)), rs1("zero"), rs2("zero"), rd("zero") {

  format = getFormat(mnemonic);

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

void Operation::genBin() {
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

std::string Operation::print_pretty() {
  const int indent = 6;
  std::stringstream ss;
  ss << hex(address) << " | " << hex(getBin()) << " | ";
  ss << "    " << red(left(mnemonic, indent));
  int imm_indent = 2;
  for(auto arg_type : format.operand) {
    if(arg_type == "rd") imm_indent--, ss << blue(left(rd, indent));
    if(arg_type == "rs1") imm_indent--, ss << blue(left(rs1, indent));
    if(arg_type == "rs2") imm_indent--, ss << blue(left(rs2, indent));
    if(arg_type == "imm") ss << std::string(imm_indent * indent, ' ') << imm.print_pretty();
  }
  return ss.str();
}

std::string Operation::print_format() {
  const int indent = 6;
  std::stringstream ss;
  ss << "    " << left(mnemonic, indent);
  for(auto arg_type : format.operand) {
    if(arg_type == "rd") ss << left(rd, indent);
    if(arg_type == "rs1") ss << left(rs1, indent);
    if(arg_type == "rs2") ss << left(rs2, indent);
    if(arg_type == "imm") ss << imm.print_format();
  }
  return ss.str();
}

Label& findLabel(std::vector<Label> vec, std::string name) {
  for(auto& label : vec)
    if(label.is(name)) return label;
  throw new std::string("Undefined label: " + name);
}

void Operation::resoluteLabel(std::vector<Label> labels) {
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
