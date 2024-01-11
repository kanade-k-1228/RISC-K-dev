#include "asmline.hpp"
#include "arch/arch.hpp"
#include "utils/utils.hpp"
#include <iomanip>
#include <regex>
#include <sstream>

ASMLine::ASMLine(const Position position, const std::string str, const uint16_t pc)
    : position(position), str(str) {
  // コメントを分離
  const auto pos = str.find(";");  // 最初にヒットした ; の位置
  if(pos != std::string::npos) {
    // comment = Comment{pos, str.substr(pos)};
    comment.emplace(pos, str.substr(pos));
  } else {
    comment = std::nullopt;
  }

  const auto splited = split(str.substr(0, pos), ' ');
  const auto head = (splited.size() == 0 ? std::nullopt : std::optional<std::string>(splited.at(0)));

  // 行を分類
  if(splited.size() == 0) {
    content = std::monostate();
  } else if(Instruction::match(splited)) {
    content = Instruction(pc, splited);
  } else if(Label::match(splited)) {
    content = Label(splited, pc);
  } else {
    throw new std::string("Undefined statement");
  }
}
