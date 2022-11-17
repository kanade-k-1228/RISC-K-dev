#include "../rkisa/rkisa.hpp"
#include "cpu.hpp"
#include "utils.hpp"
#include <fstream>
#include <set>
#include <unistd.h>

class BreakPoints : public std::set<uint16_t> {
public:
  void load(std::string fname) {
    std::ifstream fin(fname);
    if(!fin) error("Cant Open File: " + fname);
    for(std::string line; std::getline(fin, line);) {
      this->insert(std::stoi(line, nullptr, 0));
    }
  }
  bool contain(uint16_t pc) {
    return this->find(pc) != this->end();
  }
};

int main(int argc, char* argv[]) {
  CPU cpu;
  BreakPoints break_points;
  bool use_bkp = false;

  if(argc < 2) error("rkasm [.rk.bin] (.bp)");
  if(argc >= 2) cpu.load_rom(argv[1]);  // バイナリの読み込み
  if(argc >= 3) {                       // ブレークポイントの読み込み
    use_bkp = true;
    break_points.load(argv[2]);
  }

  std::cout << "----------------------------------------" << std::endl
            << "Emulate: " << argv[1] << (use_bkp ? " (Break: " + (std::string)argv[2] + ")" : "") << std::endl
            << "----------------------------------------" << std::endl;

  // 実行
  while(true) {
    bool is_break_point = use_bkp && break_points.contain(cpu.pc);
    cpu.step();
    if(is_break_point) cpu.dump();
    sleep(1);
  }
  return 0;
}
