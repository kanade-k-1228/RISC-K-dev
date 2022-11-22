#include "debug.hpp"
#include "../rkisa/rkisa.hpp"
#include "../utils/utils.hpp"
#include <fstream>
#include <regex>
#include <string>

void DebugPoints::load_dbg(std::string fname) {
  std::ifstream fin(fname);
  if(!fin) error("Cant Open File: " + fname);
  for(std::string line; std::getline(fin, line);) {
    auto tokens = split(line, ' ');
    Debug debug;
    uint16_t pc = std::stoi(tokens.at(0), nullptr, 0);
    debug.dump = true;  // 必ずダンプを吐く
    // オプション
    for(auto token : tokens) {
      if(token.at(0) == 'i') {
        debug.interrupt = true;
        debug.inter_no = token.at(1) - '0';
      }
    }
    this->insert(std::make_pair(pc, debug));
  }
}

bool DebugPoints::contain(uint16_t pc) {
  return this->find(pc) != this->end();
}

bool DebugPoints::dump(uint16_t pc) {
  return this->contain(pc) && this->at(pc).dump;
}

int DebugPoints::interrupt(uint16_t pc) {
  return (this->contain(pc) && this->at(pc).interrupt) ? this->at(pc).interrupt : -1;
}
