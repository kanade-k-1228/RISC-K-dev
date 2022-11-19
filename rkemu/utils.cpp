#include "utils.hpp"
#include <iomanip>
#include <iostream>
#include <sstream>

void error(std::string message) {
  std::cout << "------------------------------------------------" << std::endl;
  std::cout << "\033[31m ERROR! \033[m " << message << std::endl;
  std::cout << "------------------------------------------------" << std::endl;
  exit(EXIT_FAILURE);
}

std::string hex(const uint16_t n) {
  std::ostringstream ss;
  ss.setf(std::ios::hex, std::ios::basefield);
  const char fill_save = ss.fill('0');
  ss << std::setw(4) << n;
  ss.fill(fill_save);
  return ss.str();
}

std::string cprint(const std::string str, Collor collor, int width) {
  std::ostringstream ss;
  ss << "\033[" << collor << "m" << std::setw(width) << str << "\033[m";
  return ss.str();
}
