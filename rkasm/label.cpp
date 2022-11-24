#include "label.hpp"
#include "../utils/utils.hpp"

void LabelTable::define(std::string label, uint16_t address) {
  if(this->contains(label))
    error(" Find Same Label \"" + label + "\"");
  this->insert(std::map<std::string, uint16_t>::value_type(label, address));
}

uint16_t LabelTable::use(std::string label) {
  const auto lab = this->find(label);
  if(lab == this->end())
    error("Cannot find def of label \"" + label + "\"");
  return lab->second;
}
