#include "objects/objects.hpp"
#include "utils/utils.hpp"
#include <iomanip>
#include <sstream>
#include <string>

std::string printPretty(ASMLine& line) {
  std::stringstream ss;
  ss << std::setw(4) << line.position.line << " | ";
  if(line.isOperation()) ss << printPretty(line.getOperation());
  if(line.isLabel()) ss << "     |           | " << printPretty(line.getLabel(), false);
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
  for(auto [key, str, val, type, labopt] : opr.argResolved) {
    if(type == ArgType::Reg) imm_indent--, ss << blue(left(str, indent));
    if(type == ArgType::Imm) ss << std::string(imm_indent * indent, ' ') << yellow(hex((uint16_t)val));
    if(type == ArgType::Lab) {
      auto lab = labopt.value();
      ss << std::string(imm_indent * indent, ' ') << printPretty(lab, true);
    }
  }
  return ss.str();
}

std::string printPretty(Label& lab, bool opt) {
  std::stringstream ss;
  if(lab.isConst()) ss << yellow(hex(lab.value) + " = " + lab.name);
  if(lab.isVar()) ss << blue(hex(lab.value) + " = " + lab.name);
  if(lab.isOpr()) ss << (opt ? green(hex(lab.value) + " : " + lab.name) : green(lab.name + ":"));
  return ss.str();
}
