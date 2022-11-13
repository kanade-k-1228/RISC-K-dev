#pragma once
#include <string>
#include <vector>

enum Collor {
  RED = 31,
  BLUE = 34,
  GREEN = 32,
  YELLOW = 33,
};

void error(std::string message);
std::string hex(const uint16_t);
std::string hex(const uint32_t);
std::string cprint(const std::string, Collor, int);
std::vector<std::string> split(const std::string&, const char);
bool is_empty(const std::string&);
std::string trim_comment(std::string);
