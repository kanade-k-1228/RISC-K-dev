#include "../rkisa/rkisa.hpp"
#include "../utils/utils.hpp"
#include "cpu.hpp"
#include "debug.hpp"
#include <fstream>
#include <iostream>
#include <set>
#include <unistd.h>

std::string esc_char(char);

int main(int argc, char* argv[]) {
  CPU cpu;
  DumpPoints dump_points;
  IntrPoints intr_points;
  bool print_opr = false;
  bool interval_time = false;

  // Init Emulator with Comandline options
  opterr = 0;
  for(int opt; (opt = getopt(argc, argv, "d:i:ota")) != -1;) {
    if(opt == 'd') dump_points.init(optarg);
    if(opt == 'i') intr_points.init(optarg);
    if(opt == 'o') print_opr = true;
    if(opt == 't') interval_time = true;
    if(opt == 'a') dump_points.all = true;
    // if(opt == '?') {
    //   std::cout << "rkemu [-o] [-t] [-s] [-a] [-d .dump] [-i .intr] .rk.bin" << std::endl
    //             << "   t: interval Time         逐次実行します" << std::endl
    //             << "   o: print Operatioin      命令を表示します" << std::endl
    //             << "   a: dump All              全命令でダンプします" << std::endl
    //             << "   d: Dump points file      ダンプ設定ファイル" << std::endl
    //             << "   i: Interrupt points file 割り込み設定ファイル" << std::endl
    //             << "   ?: option mannual" << std::endl;
    //   return EXIT_SUCCESS;
    // }
  }
  cpu.load_rom(argv[optind]);

  std::cout << "+----------------------------------+" << std::endl
            << "|                                  |\r"
            << "| Emulate: " << cpu.fname << std::endl;
  if(dump_points.use)
    std::cout << "|                                  |\r"
              << "|  - Dump: " << dump_points.fname << std::endl;
  if(intr_points.use)
    std::cout << "|                                  |\r"
              << "|  - Intr: " << intr_points.fname << std::endl;
  std::cout << "+----------------------------------+" << std::endl;

  // Run Emulator
  for(int t = 0;; ++t) {
    uint16_t pc = cpu.pc;
    uint32_t code = cpu.rom.at(pc);

    // Execute
    cpu.execute(code);
    cpu.mem.at(ZERO) = 0;
    int sout = cpu.serial();
    bool exit = cpu.cstop();

    bool dump = dump_points.is_dump(pc);

    // Print Debug Info
    if(print_opr || dump)
      std::cout << "[" << hex(false, (uint16_t)t) << "]  "
                << cprint(hex(false, pc), GREEN, 0)
                << Debug::print_code(code)
                << ((sout != -1) ? " > " + esc_char((char)sout) : "")
                << std::endl;
    else if(sout != -1)
      std::cout << (char)sout;

    if(dump) std::cout << Debug::dump(cpu, dump_points.at(pc));

    // Interrupt
    cpu.jump_interrupt();
    cpu.catch_interrupt();
    if(intr_points.is_intr(t)) cpu.external_interrupt(intr_points.at(t).ino);

    // Exit
    if(exit) break;

    if(interval_time) usleep(10000);
  }
  std::cout << std::endl
            << "====================================" << std::endl;
  return 0;
}

std::string esc_char(char c) {
  if(c == '\n') return "\\n";
  if(c == '\0') return "\\0";
  return std::string({c});
}
