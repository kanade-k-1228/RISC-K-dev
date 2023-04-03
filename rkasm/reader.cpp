#include "reader.hpp"
#include "utils.hpp"
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
}  // namespace Reader
