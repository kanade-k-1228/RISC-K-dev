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
    = "rkemu [-o] [-a] [-t NUM] [-d FILE.dl] [-e FILE.el] FILE.rk.bin\n"
      "  -o:      print Operatioin     命令を表示\n"
      "  -a:      dump All operation   全命令ダンプ\n"
      "  -t NUM:  Terminal step        エミュレートの終了ステップ\n"
      "  -d FILE: Dump list            ダンプ情報\n"
      "  -e FILE: External event list  非同期外部イベント\n"
      "     FILE: RISC-K binary file   バイナリファイル";

int main(int argc, char* argv[]) {
  CPU cpu;
  DumpPoints dump_points;
  IntrPoints intr_points;
  int terminal_step = -1;  // opt arg t
  bool print_opr = false;  // opt o
  bool dump_all = false;   // opt a

  // Init Emulator with Comandline options
  opterr = 0;
  for(int opt; (opt = getopt(argc, argv, "oat:d:e:")) != -1;) {
    if(opt == 'o') print_opr = true;
    if(opt == 'a') dump_all = true;
    if(opt == 't') terminal_step = std::stoi(optarg);
    if(opt == 'd') dump_points.init(optarg);
    if(opt == 'e') intr_points.init(optarg);
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
  std::cout << "+-----------------------------------------------+" << std::endl
            << "| Emulate: " << left(cpu.fname, 37) << "|" << std::endl;
  if(dump_points.use) std::cout << "|  - Dump: " << left(dump_points.fname, 37) << "|" << std::endl;
  if(dump_points.all) std::cout << "|  - Dump: All                                |" << std::endl;
  if(intr_points.use) std::cout << "|  - Evnt: " << left(intr_points.fname, 37) << "|" << std::endl;
  std::cout << "+-----------------------------------------------+" << std::endl;

  // Run Emulator
  for(int t = 0;; ++t) {
    uint16_t pc = cpu.ram.get(Reg::pc);
    uint32_t code = cpu.rom.at(pc);

    // Execute
    cpu.execute(code);
    int sout = cpu.serial();

    // Print Operation
    if(print_opr || dump_all || dump_points.is_dump(pc))
      std::cout << "[" << hex((uint16_t)t) << "] " << green(hex(pc)) << " : " << Debug::print_code(code)
                << ((sout != -1) ? " > " + esc_char((char)sout) : "") << std::endl;
    else if(sout != -1)
      std::cout << (char)sout;

    // Print Registor Value
    if(dump_points.is_dump(pc))
      std::cout << Debug::dump(cpu, dump_points.at(pc));
    else if(dump_all)
      std::cout << Debug::dump_reg(cpu);

    // Catch external pin
    // Interrupt
    cpu.interrupt(intr_points.is_intr(t) ? intr_points.at(t).irq_flag : 0);

    // Exit Emulator
    if(cpu.is_shutdowned() || (terminal_step != -1 && t > terminal_step)) break;
  }
  std::cout << std::endl
            << "=================================================" << std::endl;
  return 0;
}

std::string esc_char(char c) {
  if(c == '\n') return "\\n";
  if(c == '\0') return "\\0";
  return std::string({c});
}
