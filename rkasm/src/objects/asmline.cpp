#include "asmline.hpp"
#include "arch/arch.hpp"
#include "utils/utils.hpp"
#include <iomanip>
#include <regex>
#include <sstream>

bool match_label(std::string s) { return s.front() == '@' || s.front() == '#' || s.back() == ':'; }

ASMLine::ASMLine(const Position position, const std::string str, const uint16_t pc)
    : position(position), str(str) {
  // コメントを分離
  const auto pos = str.find(";");  // 最初にヒットした ; の位置
  if(pos != std::string::npos) {
    comment.emplace(Comment{pos, str.substr(pos)});
  } else {
    comment = std::nullopt;
  }
  const auto splited = split(str.substr(0, pos), ' ');
  // 行を分類
  if(splited.size() == 0) {
    content = std::monostate();
  } else if(is_mnemonic(splited.at(0))) {
    content = Operation(pc, splited);
  } else if(match_label(splited.at(0))) {
    content = Label(pc, splited);
  } else {
    throw new std::string("Undefined statement");
  }
}
