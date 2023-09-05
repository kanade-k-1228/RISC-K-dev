#include "utils.hpp"
#include <iomanip>
#include <iostream>
#include <regex>
#include <sstream>

std::string red(const std::string str) { return "\e[31m" + str + "\e[m"; };
std::string green(const std::string str) { return "\e[32m" + str + "\e[m"; };
std::string yellow(const std::string str) { return "\e[33m" + str + "\e[m"; };
std::string blue(const std::string str) { return "\e[34m" + str + "\e[m"; };
std::string magenta(const std::string str) { return "\e[35m" + str + "\e[m"; };
std::string cyan(const std::string str) { return "\e[36m" + str + "\e[m"; };

std::string right(const std::string str, int spacing) {
  std::stringstream ss;
  ss << std::setw(spacing) << str;
  return ss.str();
}

std::string hex(const uint8_t n) {
  std::ostringstream ss;
  ss.setf(std::ios::hex, std::ios::basefield);
  const char fill_save = ss.fill('0');
  ss << std::setw(2) << n;
  ss.fill(fill_save);
  return ss.str();
}

std::string hex(const uint16_t n) {
  std::ostringstream ss;
  ss.setf(std::ios::hex, std::ios::basefield);
  const char fill_save = ss.fill('0');
  ss << std::setw(4) << n;
  ss.fill(fill_save);
  return ss.str();
}

std::string hex(const uint32_t n) {
  std::ostringstream ss;
  ss.setf(std::ios::hex, std::ios::basefield);
  const char fill_save = ss.fill('0');
  ss << std::setw(4) << ((n >> 16) & 0xffff)
     << "_" << std::setw(4) << (n & 0xffff);
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

std::string print_error(std::string fname, int line_cnt, std::string line, std::string msg) {
  std::stringstream ss;
  std::string place = fname + ":" + std::to_string(line_cnt);
  ss << std::string(place.size(), ' ') << " | " << std::endl
     << place << " | " << line << std::endl
     << std::string(place.size(), ' ') << " | \033[31m ERROR! \033[m " << msg << std::endl;
  return ss.str();
}
