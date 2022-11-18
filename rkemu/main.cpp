#include "../rkisa/rkisa.hpp"
#include "breakPoint.hpp"
#include "cpu.hpp"
#include "utils.hpp"
#include <fstream>
#include <set>
#include <unistd.h>

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
    usleep(100000);
  }
  return 0;
}
