#include "line.hpp"
#include "../isa.hpp"
#include "../utils.hpp"
#include <regex>

bool isMnemonic(std::string s) {
  for(auto& code : isa) {
    if(code.mnemonic == s) return true;
  }
  return false;
}

Line::Line(const std::string file, const int line_no, const std::string str, const uint16_t pc)
    : file(file), line(line_no), str(str) {

  // コメントを分離
  const auto pos = str.find(";");  // 最初にヒットした ; の位置
  if(pos != std::string::npos) {
    this->comment = str.substr(pos);
    this->splited = split(str.substr(0, pos), ' ');
  } else {
    this->comment = "";
    this->splited = split(str, ' ');
  }

  // 行の分類
  if(this->splited.size() == 0) {
    this->type = VOID;
  } else if(isMnemonic(this->splited.at(0))) {
    this->type = OPERATION;
    this->operation = Operation(pc, splited);
  } else if(this->splited.at(0).front() == '@' || this->splited.at(0).front() == '#' || this->splited.at(0).back() == ':') {
    type = LABEL;  // ラベル定義
    label = Label(pc, splited);
  } else {
    throw new std::string("Undefined statement");
    return;
  }
}
