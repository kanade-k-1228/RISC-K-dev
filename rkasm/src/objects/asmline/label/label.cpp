#include "label.hpp"
#include "utils/utils.hpp"
#include <algorithm>
#include <sstream>
#include <string>

Label::Label(const std::vector<std::string>& splited, const uint16_t program_address) {
  if(OprLabel::match(splited)) {
    label.emplace<OprLabel>(splited, program_address);
  } else if(VarLabel::match(splited)) {
    label.emplace<VarLabel>(splited, program_address);
  } else if(ConstLabel::match(splited)) {
    label.emplace<ConstLabel>(splited, program_address);
  } else {
    throw new std::string("Invalid label: " + splited.at(0));
  }
}

Label& findLabel(std::vector<Label>& vec, std::string name) {
  auto result = std::find_if(vec.begin(), vec.end(), [name](Label l) { return l.getName() == name; });
  if(result == vec.end()) {
    throw new std::string("Undefined label: " + name);
  } else {
    return *result;
  }
}
