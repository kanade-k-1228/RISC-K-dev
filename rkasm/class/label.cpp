#include "label.hpp"
#include "../utils.hpp"
#include <sstream>
#include <string>

Label::Label(const uint16_t address, const std::vector<std::string> str) {
  std::string str0 = str.at(0);
  if(str0.back() == ':') {  // 命令ラベル hoge:
    type = OPR;
    str0.pop_back();
    name = str0;
    value = address;
  } else if(str0.front() == '@') {  // 変数ラベル @0x00 hoge
    type = VAR;
    name = str.at(1);
    str0.erase(str0.begin());
    value = std::stoi(str0, nullptr, 0);
  } else if(str0.front() == '#') {  // 定数ラベル #0x00 hoge
    type = CONST;
    name = str.at(1);
    str0.erase(str0.begin());
    value = std::stoi(str0, nullptr, 0);
  }
}

std::string Label::print() {
  std::stringstream ss;
  if(isConst()) ss << cprint(hex(true, value), YELLOW, 0) << " = " << name;
  if(isVar()) ss << cprint(hex(true, value), BLUE, 0) << " = " << name;
  if(isOpr()) ss << cprint(hex(true, value), GREEN, 0) << cprint(": " + name, GREEN, 0);
  return ss.str();
}
