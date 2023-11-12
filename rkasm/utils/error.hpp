#include <optional>
#include <string>
#include <variant>

struct Position {
  std::string file;
  int line;
};

std::string print_error(Position pos, std::string line, std::string msg);
