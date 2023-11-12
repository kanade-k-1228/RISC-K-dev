#include "objects/objects.hpp"
#include "utils/utils.hpp"
#include <iomanip>
#include <sstream>
#include <string>

std::string printPretty(ASMLine& line) {
  std::stringstream ss;
  ss << std::setw(4) << line.position.line << " | ";
  if(line.isOperation()) ss << printPretty(line.getOperation());
  if(line.isLabel()) ss << "     |           | " << printPretty(line.getLabel());
  if(line.isVoid()) ss << "     |           | ";
  ss << (line.comment.has_value() ? line.comment.value().comment : "");
  return ss.str();
}

std::string printPretty(Instruction& opr) {
  const int indent = 6;
  std::stringstream ss;
  ss << hex(opr.address) << " | " << hex(opr.getBin()) << " | ";
  ss << "    " << red(left(opr.mnemonic, indent));
  int imm_indent = 2;
  for(auto arg_type : opr.format.operand) {
    if(arg_type == "rd") imm_indent--, ss << blue(left(opr.rd, indent));
    if(arg_type == "rs1") imm_indent--, ss << blue(left(opr.rs1, indent));
    if(arg_type == "rs2") imm_indent--, ss << blue(left(opr.rs2, indent));
    if(arg_type == "imm") ss << std::string(imm_indent * indent, ' ') << printPretty(opr.imm);
  }
  return ss.str();
}

std::string printPretty(Label& lab) {
  std::stringstream ss;
  if(lab.isConst()) ss << yellow(hex(lab.value) + " = " + lab.name);
  if(lab.isVar()) ss << blue(hex(lab.value) + " = " + lab.name);
  if(lab.isOpr()) ss << green(lab.name + ":");
  return ss.str();
}

std::string printPretty(Imm& imm) {
  if(imm.type == Imm::LITERAL) return yellow(hex(imm.value));
  if(imm.type == Imm::OPR_LAB_REF) return green(hex(imm.value) + " = " + imm.label);
  if(imm.type == Imm::VAR_LAB_REF) return cyan(hex(imm.value) + " = " + imm.label);
  if(imm.type == Imm::CONST_LAB_REF) return yellow(hex(imm.value) + " = " + imm.label);
  return "";
}
