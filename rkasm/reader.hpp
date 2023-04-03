#pragma once
#include <string>

namespace Reader {
extern std::string line;

void init(std::string fin_name);
bool getline();
}  // namespace Reader
