#include "line.hpp"
#include "../rkisa/rkisa.hpp"
#include "../utils/utils.hpp"
#include <regex>
Line::Line(const uint16_t address, const std::vector<std::string> str)
    : operation(address, str), label_def(address, str) {
  if(std::regex_search(str.at(0), std::regex("^(" + mnemonic + "|" + mnemonic_pseudo + ")$"))) {
    type = OPERATION;  // 命令
  } else if(std::regex_search(str.at(0), std::regex("(:$)|(^@)|(^#)"))) {
    type = LABEL_DEF;  // ラベル定義
  } else {
    error("Unknown Code");
  }
}
