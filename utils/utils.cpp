#include "utils.hpp"
#include "../rkisa/rkisa.hpp"
#include <iomanip>
#include <iostream>
#include <regex>
#include <sstream>

void error(std::string message) {
  std::cout << std::endl
            << "------------------------------------------------" << std::endl;
  std::cout << "\033[31m ERROR! \033[m " << message << std::endl;
  // std::cout << std::setw(6) << line_cnt << ":" << line << std::endl;
  std::cout << "------------------------------------------------" << std::endl;
  exit(EXIT_FAILURE);
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

uint16_t decode_imm(uint32_t code, uint16_t opc) {
  if(opc == CALC) return 0;
  if(opc == CALCI) return ((code >> 10) & 0x03ff) | (((code >> 24) & 0x0003) << 10);
  if(opc == LOAD || opc == LOADI || opc == JUMP) return (code >> 10) & 0xffff;
  if(opc == STORE || opc == BREQ || opc == BRLT) return ((code >> 10) & 0x03ff) | ((code >> 26) & 0x003f);
  error("Unknown OPC: " + opc);
  return 0;
}

uint16_t decode_func(uint32_t code, uint16_t opc) {
  if(opc == CALC) return (code >> 10) & 0x000f;
  if(opc == CALCI) return (code >> 20) & 0x000f;
  return 0;
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

bool is_empty(const std::string& str) {
  const std::regex non_empty(R"(\S)");  // 空でない文字にヒットする正規表現
  return !std::regex_search(str, non_empty);
}

std::string trim_comment(std::string str) {
  const auto pos = str.find(";");  // 最初にヒットした ; の位置
  if(pos != std::string::npos)     // npos はヒットしなかった場合の pos の値
    str = str.erase(pos);          // pos 以降を削除
  return str;
}
