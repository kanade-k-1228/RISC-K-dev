#include <map>
#include <vector>

class LabelTable : public std::map<std::string, uint16_t> {
public:
  void define(std::string, uint16_t);
  uint16_t get_value(std::string);
  std::vector<std::pair<uint16_t, std::string>> sort_by_value();
};
