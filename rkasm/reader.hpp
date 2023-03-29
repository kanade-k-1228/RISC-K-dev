#pragma once
#include <string>

namespace Reader {
extern std::string line;

void init(std::string fin_name);
bool getline();
void error(std::string message);
void warn(std::string message);
}  // namespace Reader
