#include <set>
#include <string>

class BreakPoints : public std::set<uint16_t> {
public:
  void load(std::string);
  bool contain(uint16_t);
};
