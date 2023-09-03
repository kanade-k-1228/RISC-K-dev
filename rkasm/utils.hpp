#pragma once
#include <string>
#include <tuple>
#include <vector>

extern std::tuple<std::string, int, std::string> error_notation;

void error(std::string message);
void warn(std::string message);

enum Collor {
  RED = 31,
  BLUE = 34,
  GREEN = 32,
  YELLOW = 33,
};

std::string cprint(const std::string, Collor, int);

std::string hex(bool, const uint16_t);
std::string hex(bool, const uint32_t);

std::vector<std::string> split(const std::string&, const char);

bool include(std::vector<std::string>, std::string);
