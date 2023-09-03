#include "label_table.hpp"
#include "../utils/utils.hpp"
#include <algorithm>

Label* LabelTable::get(std::string label) {
  for(auto& l : *this) {
    if(l.name == label) {
      return &l;
    }
  }
  return nullptr;
}
