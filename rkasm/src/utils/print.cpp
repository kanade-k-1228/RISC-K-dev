#include "print.hpp"
#include <iomanip>
#include <iostream>
#include <iterator>

std::string right(const std::string str, int spacing) {
  std::stringstream ss;
  ss << std::setw(spacing) << str;
  return ss.str();
}

std::string left(const std::string str, int spacing) {
  std::stringstream ss;
  ss << std::setw(spacing) << std::left << str;
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

std::ostream& operator<<(std::ostream& os, const std::vector<std::string>& v) {
  std::copy(v.begin(), v.end(), std::ostream_iterator<std::string>(os, " "));
  return os;
}
