#include "label.hpp"
#include "../utils/utils.hpp"
#include <algorithm>

void LabelTable::define(std::string label, uint16_t address) {
  if(this->contains(label))
    error(" Find Same Label \"" + label + "\"");
  this->insert(std::map<std::string, uint16_t>::value_type(label, address));
}

uint16_t LabelTable::get_value(std::string label) {
  const auto lab = this->find(label);
  if(lab == this->end())
    error("Cannot find def of label \"" + label + "\"");
  return lab->second;
}

std::vector<std::pair<uint16_t, std::string>> LabelTable::sort_by_value() {
  std::vector<std::pair<uint16_t, std::string>> ret(0);
  for(auto label : *this) ret.push_back(std::make_pair(label.second, label.first));
  std::sort(ret.begin(), ret.end());
  return ret;
}
