#pragma once
#include <functional>
#include <string>
#include <tuple>
#include <vector>

std::string red(const std::string str);
std::string green(const std::string str);
std::string yellow(const std::string str);
std::string blue(const std::string str);
std::string magenta(const std::string str);
std::string cyan(const std::string str);

std::string right(const std::string str, int spacing);

std::string hex(const uint8_t);
std::string hex(const uint16_t);
std::string hex(const uint32_t);

std::vector<std::string> split(const std::string&, const char);

std::string print_error(std::string fname, int line_cnt, std::string line, std::string msg);
