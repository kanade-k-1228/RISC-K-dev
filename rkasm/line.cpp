#include "line.hpp"
#include "isa.hpp"
#include "utils.hpp"
#include <regex>
Line::Line(const uint16_t address, const std::vector<std::string> str) {
  if(std::regex_search(str.at(0), std::regex("^(" + mnemonic + ")$"))) {
    type = OPERATION;  // 命令
    operation = Operation(address, str);
  } else if(std::regex_search(str.at(0), std::regex("(:$)|(^@)|(^#)"))) {
    type = LABEL_DEF;  // ラベル定義
    label_def = LabelDef(address, str);
  } else {
    error("Unknown Code");
  }
}
