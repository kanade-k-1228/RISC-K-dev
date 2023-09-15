#include "line.hpp"
#include "../../rkarch/arch.hpp"
#include "../utils.hpp"
#include <iomanip>
#include <regex>
#include <sstream>

bool match_label(std::string s) { return s.front() == '@' || s.front() == '#' || s.back() == ':'; }

Line::Line(const std::string file, const int line_no, const std::string str, const uint16_t pc)
    : file(file), line(line_no), str(str) {

  // コメントを分離
  const auto pos = str.find(";");  // 最初にヒットした ; の位置
  if(pos != std::string::npos) {
    comment = str.substr(pos);
    splited = split(str.substr(0, pos), ' ');
  } else {
    comment = "";
    splited = split(str, ' ');
  }

  // 行の分類
  if(this->splited.size() == 0) {
    type = VOID;
  } else if(is_mnemonic(this->splited.at(0))) {
    type = OPERATION;
    operation = Operation(pc, splited);
  } else if(match_label(this->splited.at(0))) {
    type = LABEL;
    label = Label(pc, splited);
  } else {
    throw new std::string("Undefined statement");
  }
}

std::string Line::print_pretty() {
  std::stringstream ss;
  ss << std::setw(4) << line << " | ";
  if(isOperation()) ss << operation.print_pretty() << "  " << comment;
  if(isLabel()) ss << "     |           | " << label.print_pretty() << "  " << comment;
  if(isVoid()) ss << "     |           | " << comment;
  return ss.str();
}

std::string Line::print_format() {
  std::stringstream ss;
  if(isOperation()) ss << operation.print_format() << "  " << comment;
  if(isLabel()) ss << label.print_format() << "  " << comment;
  if(isVoid()) ss << comment;
  return ss.str();
}
