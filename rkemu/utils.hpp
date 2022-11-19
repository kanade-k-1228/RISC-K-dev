#pragma once
#include <string>

enum Collor {
  RED = 31,
  BLUE = 34,
  GREEN = 32,
  YELLOW = 33,
};

void error(std::string message);
std::string hex(const uint16_t);
std::string cprint(const std::string, Collor, int);
