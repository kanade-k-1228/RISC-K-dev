#include "utils.hpp"
#include "../rkisa/rkisa.hpp"
#include <iomanip>
#include <iostream>
#include <regex>
#include <sstream>

void error(std::string message) {
  std::cout << std::endl
            << "------------------------------------" << std::endl
            << "\033[31m ERROR! \033[m " << message << std::endl
            << "------------------------------------" << std::endl;
  exit(EXIT_FAILURE);
}

void warn(std::string message) {
  std::cout << std::endl
            << "------------------------------------" << std::endl
            << "\033[33m WARN \033[m " << message << std::endl
            << "------------------------------------" << std::endl;
}

std::string cprint(const std::string str, Collor collor, int width) {
  std::ostringstream ss;
  ss << "\033[" << collor << "m" << std::setw(width) << str << "\033[m";
  return ss.str();
}

std::string hex(bool prefix, const uint16_t n) {
  std::ostringstream ss;
  ss.setf(std::ios::hex, std::ios::basefield);
  const char fill_save = ss.fill('0');
  ss << (prefix ? "0x" : "") << std::setw(4) << n;
  ss.fill(fill_save);
  return ss.str();
}

std::string hex(bool prefix, const uint32_t n) {
  std::ostringstream ss;
  ss.setf(std::ios::hex, std::ios::basefield);
  const char fill_save = ss.fill('0');
  ss << (prefix ? "0x" : "") << std::setw(8) << n;
  ss.fill(fill_save);
  return ss.str();
}

// 区切り文字 sep で分割
std::vector<std::string> split(const std::string& str, const char sep) {
  std::vector<std::string> v;
  std::stringstream ss(str);
  std::string buffer;
  while(std::getline(ss, buffer, sep)) {
    if(buffer == "") continue;  // 区切り文字が連続した場合、削除
    v.push_back(buffer);
  }
  return v;
}
