#include "reader.hpp"
#include "../utils/utils.hpp"
#include <fstream>
#include <iostream>

namespace Reader {
int line_cnt = 0;
std::string fname;
std::string line;
std::ifstream fin;

void init(std::string fin_name) {
  fname = fin_name;
  fin.open(fname, std::ios::in);
  if(!fin) error("Can't open input file: " + fname);
}

bool getline() {
  ++line_cnt;
  if(std::getline(fin, line)) {
    std::cout << "\33[2K\r" << fname << ":" << line_cnt << ":" << Reader::line;
    return true;
  } else {
    std::cout << "\33[2K\r";
    return false;
  }
}

void error(std::string message) {
  std::cout << std::endl
            << "\033[31m------------------------------------\033[m\r"
            << "\033[31m" << message << "\033[m " << std::endl
            << "\033[31m|                                  |\033[m\r"
            << "\033[31m|\033[m " << fname << ":" << line_cnt << std::endl
            << "\033[31m|                                  |\033[m\r"
            << "\033[31m|\033[m " << line << std::endl
            << "\033[31m------------------------------------\033[m" << std::endl;
  exit(EXIT_FAILURE);
}

void warn(std::string message) {
  std::cout << std::endl
            << "------------------------------------" << std::endl
            << "\033[33m WARN \033[m " << fname << ":" << line_cnt << std::endl
            << " " << line << std::endl
            << " " << message << std::endl
            << "------------------------------------" << std::endl;
}
}  // namespace Reader
