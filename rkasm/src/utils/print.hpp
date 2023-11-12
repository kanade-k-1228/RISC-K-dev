#pragma once
#include <string>
#include <vector>

inline std::string red(const std::string str) { return "\e[31m" + str + "\e[m"; };
inline std::string green(const std::string str) { return "\e[32m" + str + "\e[m"; };
inline std::string yellow(const std::string str) { return "\e[33m" + str + "\e[m"; };
inline std::string blue(const std::string str) { return "\e[34m" + str + "\e[m"; };
inline std::string magenta(const std::string str) { return "\e[35m" + str + "\e[m"; };
inline std::string cyan(const std::string str) { return "\e[36m" + str + "\e[m"; };

std::string right(const std::string str, int spacing);
std::string left(const std::string str, int spacing);

std::string hex(const uint16_t);
std::string hex(const uint32_t);

std::ostream& operator<<(std::ostream& os, const std::vector<std::string>& v);
