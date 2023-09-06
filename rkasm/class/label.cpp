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

std::string Label::print_pretty() {
  std::stringstream ss;
  if(isConst()) ss << yellow(hex(value) + " = " + name);
  if(isVar()) ss << blue(hex(value) + " = " + name);
  if(isOpr()) ss << green(name + ":");
  return ss.str();
}

std::string Label::print_format() {
  std::stringstream ss;
  if(isConst()) ss << "#0x" << hex(value) << " " << name;
  if(isVar()) ss << "@0x" << hex(value) << " " << name;
  if(isOpr()) ss << name << ":";
  return ss.str();
}
