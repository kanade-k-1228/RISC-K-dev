#pragma once
#include <functional>
#include <string>
#include <tuple>
#include <vector>

enum Collor {
  RED = 31,
  GREEN = 32,
  YELLOW = 33,
  BLUE = 34,
  MAGENTA = 35,
  CYAN = 36,
};

std::string cprint(const std::string, Collor, int);

std::string hex(bool, const uint16_t);
std::string hex(bool, const uint32_t);

std::vector<std::string> split(const std::string&, const char);

bool include(std::vector<std::string>, std::string);

template <typename T>
T* find(std::vector<T>, std::function<bool(T)>);
