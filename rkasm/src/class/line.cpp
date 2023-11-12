#include "line.hpp"
#include "arch/arch.hpp"
#include "utils/utils.hpp"
#include <iomanip>
#include <regex>
#include <sstream>

bool match_label(std::string s) { return s.front() == '@' || s.front() == '#' || s.back() == ':'; }

Line::Line(const Position position, const std::string line)
    : position(position), line(line) {
  // コメントを分離
  const auto pos = line.find(";");  // 最初にヒットした ; の位置
  if(pos != std::string::npos) {
    comment.emplace(Comment{pos, line.substr(pos)});
  } else {
    comment = std::nullopt;
  }
  const auto splited = split(line.substr(0, pos), ' ');
  // 行の分類
  if(splited.size() == 0) {
    content = std::monostate();
  } else if(is_mnemonic(splited.at(0))) {
    content = Operation(0, splited);
  } else if(match_label(splited.at(0))) {
    content = Label(0, splited);
  } else {
    throw new std::string("Undefined statement");
  }
}

std::string Line::print_pretty() {
  std::stringstream ss;
  ss << std::setw(4) << position.line << " | ";
  if(isOperation()) ss << getOperation().print_pretty();
  if(isLabel()) ss << "     |           | " << getLabel().print_pretty();
  if(isVoid()) ss << "     |           | ";
  ss << (comment.has_value() ? comment.value().comment : "");
  return ss.str();
}

// std::string ASM::print_format() {
//   std::stringstream ss;
//   if(isOperation()) ss << operation.print_format() << "  " << comment;
//   if(isLabel()) ss << label.print_format() << "  " << comment;
//   if(isVoid()) ss << comment;
//   return ss.str();
// }
