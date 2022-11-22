#include <map>
#include <string>

struct Debug {
  bool dump = false;
  bool interrupt = false;
  int inter_no = 0;
};

class DebugPoints : public std::map<uint16_t, Debug> {
public:
  void load_dbg(std::string);
  bool contain(uint16_t);
  bool dump(uint16_t);
  int interrupt(uint16_t);
};
