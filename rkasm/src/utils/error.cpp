#include "error.hpp"
#include <sstream>

std::string print_error(Position pos, std::string line, std::string msg) {
  std::stringstream ss;
  std::string place = pos.file + ":" + std::to_string(pos.line);
  ss << std::string(place.size(), ' ') << " | " << std::endl
     << place << " | " << line << std::endl
     << std::string(place.size(), ' ') << " | \033[31m ERROR! \033[m " << msg << std::endl;
  return ss.str();
}
