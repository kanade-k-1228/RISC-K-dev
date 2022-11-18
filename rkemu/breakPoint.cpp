#include "breakPoint.hpp"
#include "utils.hpp"
#include <fstream>
#include <string>

void BreakPoints::load(std::string fname) {
  std::ifstream fin(fname);
  if(!fin) error("Cant Open File: " + fname);
  for(std::string line; std::getline(fin, line);) {
    this->insert(std::stoi(line, nullptr, 0));
  }
}

bool BreakPoints::contain(uint16_t pc) {
  return this->find(pc) != this->end();
}
