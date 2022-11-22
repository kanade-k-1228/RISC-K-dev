#include "../rkisa/rkisa.hpp"
#include "../utils/utils.hpp"
#include "cpu.hpp"
#include "debug.hpp"
#include <fstream>
#include <set>
#include <unistd.h>

int main(int argc, char* argv[]) {
  // コマンドライン引数の処理
  opterr = 0;
  bool print_opc = false;
  bool cout_single_line = false;
  bool dump_all = false;
  bool interval_time = false;
  bool use_dbg = false;
  std::string dbg_file = "";
  std::string bin_file = "";
  for(int opt; (opt = getopt(argc, argv, "cndtb:")) != -1;) {
    switch(opt) {
    case 'c':
      print_opc = true;
      break;
    case 'n':
      cout_single_line = true;
      break;
    case 'd':
      dump_all = true;
      break;
    case 't':
      interval_time = true;
      break;
    case 'b':
      use_dbg = true;
      dbg_file = optarg;
      break;
    default:
      std::cout << "Usage: rkemu [-c] [-n] [-d] [-t] [-b .dbg] .rk.bin" << std::endl;
      break;
    }
  }
  bin_file = argv[optind];

  // 初期化
  CPU cpu;
  cpu.load_rom(bin_file);
  DebugPoints break_points;
  if(use_dbg) break_points.load_dbg(dbg_file);

  std::cout << "------------------------------" << std::endl
            << "Emulate: " << bin_file << (use_dbg ? " (Break: " + dbg_file + ")" : "") << std::endl
            << "------------------------------" << std::endl;

  int pc;
  int intr_no;
  // 実行
  while(true) {
    pc = cpu.pc;

    // 実行
    cpu.execute(cpu.rom.at(pc));
    cpu.mem.at(ZERO) = 0;
    cpu.serial(cout_single_line);
    cpu.cstop();

    // Debug
    if(print_opc)
      cpu.print_code(pc, cpu.rom.at(pc));
    if(dump_all || (break_points.contain(pc) && break_points[pc].dump))
      cpu.dump();
    if(-1 != (intr_no = break_points.interrupt(pc)))
      cpu.interrupt(intr_no);
    if(interval_time)
      usleep(10000);
  }
  return 0;
}
