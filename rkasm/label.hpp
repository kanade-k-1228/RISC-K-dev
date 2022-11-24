#include <map>

class LabelTable : public std::map<std::string, uint16_t> {
public:
  void define(std::string, uint16_t);
  uint16_t use(std::string);
};
