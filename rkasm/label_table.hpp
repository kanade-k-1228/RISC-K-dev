#include "label_def.hpp"
#include <map>
#include <string>
#include <vector>

class LabelTable : public std::vector<Label> {
public:
  Label& get(std::string label);
};
