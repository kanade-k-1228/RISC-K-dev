#include "stoios.hpp"
#include <iostream>

std::variant<std::string, int> stoios(std::string str) {
  try {
    int ret = std::stoi(str, nullptr, 0);
    return ret;
  } catch(std::out_of_range& e) {
    throw new std::string("Immidiate Out of Range: " + str);
  } catch(std::invalid_argument& e) {
    return str;
  }
}
