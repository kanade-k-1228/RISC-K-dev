#include "debug.hpp"
#include "model.hpp"
#include "utils.hpp"
#include <fstream>
#include <iostream>
#include <ncurses.h>
#include <set>
#include <unistd.h>

std::string esc_char(char);

std::string man
    = "rkemu [-o] [-t] [-s] [-a] [-d .dump] [-i .intr] .rk.bin\n"
      "  -t:      set interval Time     命令実行の間で少し待ちます\n"
      "  -s:      Step by step          命令を逐次実行します\n"
      "  -o:      print Operatioin      命令を表示します\n"
      "  -a:      dump All operation    全命令でダンプします\n"
      "  -d FILE: Dump points file      ダンプ時刻ファイル\n"
      "  -i FILE: Interrupt points file 割り込み時刻ファイル\n"
      "     FILE: Emulation binary      エミュレートするバイナリファイル";

int main(int argc, char* argv[]) {
  CPU cpu;
  DumpPoints dump_points;
  IntrPoints intr_points;
  bool print_opr = false;
  bool interval_time = false;
  bool dump_all = false;
  bool step_by_step = false;

  // Init Emulator with Comandline options
  opterr = 0;
  for(int opt; (opt = getopt(argc, argv, "d:i:otas")) != -1;) {
    if(opt == 'd') dump_points.init(optarg);
    if(opt == 'i') intr_points.init(optarg);
    if(opt == 'o') print_opr = true;
    if(opt == 't') interval_time = true;
    if(opt == 'a') dump_all = true;
    if(opt == 's') step_by_step = true;
    if(opt == '?') {
      std::cout << man << std::endl;
      return EXIT_FAILURE;
    }
  }
  if(!(optind < argc)) {
    std::cout << man << std::endl;
    return EXIT_FAILURE;
  }
  cpu.load_rom_file(argv[optind]);

  // Print Emulation Conditions
  std::cout << "+----------------------------------------+" << std::endl
            << "|                                        |\r"
            << "| Emulate: " << cpu.fname << std::endl;
  if(dump_points.use)
    std::cout << "|                                        |\r"
              << "|  - Dump: " << dump_points.fname << std::endl;
  if(dump_points.all)
    std::cout << "|  - Dump: All                           |" << std::endl;
  if(intr_points.use)
    std::cout << "|                                        |\r"
              << "|  - Intr: " << intr_points.fname << std::endl;
  std::cout << "+----------------------------------------+" << std::endl;

  // Run Emulator
  for(int t = 0;; ++t) {
    uint16_t pc = cpu.ram.get(Reg::pc);
    uint32_t code = cpu.rom.at(pc);

    // Execute
    cpu.execute(code);
    int sout = cpu.serial();
    bool exit = cpu.is_shutdowned();

    // Print Operation
    if(print_opr || dump_all || dump_points.is_dump(pc))
      std::cout << "[" << hex(false, (uint16_t)t) << "]  "
                << cprint(hex(false, pc), GREEN, 0)
                << Debug::print_code(code)
                << ((sout != -1) ? " > " + esc_char((char)sout) : "")
                << std::endl;
    else if(sout != -1)
      std::cout << (char)sout;

    // Print Registor Value
    if(dump_points.is_dump(pc))
      std::cout << Debug::dump(cpu, dump_points.at(pc));
    else if(dump_all)
      std::cout << Debug::dump(cpu);

    // Interrupt
    cpu.jump_interrupt();
    cpu.catch_interrupt();
    if(intr_points.is_intr(t))
      cpu.external_irq(intr_points.at(t).ino);

    // Goto Next Operation
    if(step_by_step) {
      char cmd = getch();
      while(!cmd) {
        if(cmd == '\n') continue;
        if(cmd == '\e') break;
        if(cmd == 'i') {
          std::cout << "Interrupt No ? ";
          std::cin >> cmd;
          int intr_no = cmd - '0';
          cpu.external_irq(intr_no);
        }
      }
    }

    // Exit Emulator
    if(exit) break;

    // Interval Time between Operation
    if(interval_time) usleep(10000);
  }
  std::cout << std::endl
            << "==========================================" << std::endl;
  return 0;
}

std::string esc_char(char c) {
  if(c == '\n') return "\\n";
  if(c == '\0') return "\\0";
  return std::string({c});
}
