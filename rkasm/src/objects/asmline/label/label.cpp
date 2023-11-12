#include "label.hpp"
#include "utils/utils.hpp"
#include <algorithm>
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

bool Label::match(std::vector<std::string> splited) {
  auto s = splited.at(0);
  return s.front() == '@' || s.front() == '#' || s.back() == ':';
}

Label& findLabel(std::vector<Label> vec, std::string name) {
  auto result = std::find_if(vec.begin(), vec.end(), [name](Label l) { return l.is(name); });
  if(result == vec.end()) {
    throw new std::string("Undefined label: " + name);
  } else {
    return *result;
  }
}
