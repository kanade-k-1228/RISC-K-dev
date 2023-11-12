#include "stoi.hpp"
#include <iostream>

std::variant<std::string, int> stoios(std::string str) {
  try {
    int ret = std::stoi(str, nullptr, 0);
    return ret;
  } catch(std::out_of_range& e) {
    throw e;
  } catch(std::invalid_argument& e) {
    return str;
  }
}

std::optional<int> stoi_opt(std::string str) {
  try {
    return std::stoi(str, nullptr, 0);
  } catch(...) {
    return std::nullopt;
  }
}
