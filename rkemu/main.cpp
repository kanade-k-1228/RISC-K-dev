#include "../rkisa/rkisa.hpp"
#include "breakPoint.hpp"
#include "cpu.hpp"
#include "utils.hpp"
#include <fstream>
#include <set>
#include <unistd.h>

int main(int argc, char* argv[]) {
  // コマンドライン引数の処理
  opterr = 0;
  bool debug = false;
  bool use_bp = false;
  std::string bp_file = "";
  std::string bin_file = "";
  for(int opt; (opt = getopt(argc, argv, "db:")) != -1;) {
    switch(opt) {
    case 'd':
      debug = true;
      break;
    case 'b':
      use_bp = true;
      bp_file = optarg;
      break;
    default:
      std::cout << "Usage: rkemu [-d] [-b fname] file" << std::endl;
      break;
    }
  }
  bin_file = argv[optind];

  CPU cpu(bin_file);
  cpu.debug = debug;
  BreakPoints break_points;
  if(use_bp) break_points.load(bp_file);

  std::cout
      << "------------------------------" << std::endl
      << "Emulate: " << bin_file << (use_bp ? " (Break: " + bp_file + ")" : "") << std::endl
      << "------------------------------" << std::endl;

  // 実行
  while(true) {
    bool dump = debug || (use_bp && break_points.contain(cpu.pc));
    cpu.step();
    if(dump) cpu.dump();
    usleep(100000);
  }
  return 0;
}
