#include "../rkisa/rkisa.hpp"
#include "../utils/utils.hpp"
#include "cpu.hpp"
#include "debug.hpp"
#include <fstream>
#include <iostream>
#include <set>
#include <unistd.h>

int main(int argc, char* argv[]) {
  CPU cpu;
  DumpPoints dump_points;
  IntrPoints intr_points;
  bool dump_all = false;
  bool print_code = false;
  bool cout_single_line = false;
  bool interval_time = false;

  // Init Emulator with Comandline options
  opterr = 0;
  for(int opt; (opt = getopt(argc, argv, "acd:i:nt")) != -1;) {
    if(opt == 'a') dump_all = true;
    if(opt == 'c') print_code = true;
    if(opt == 'd') dump_points.init(optarg);
    if(opt == 'i') intr_points.init(optarg);
    if(opt == 'n') cout_single_line = true;
    if(opt == 't') interval_time = true;
  }
  cpu.load_rom(argv[optind]);

  std::cout << "------------------------------------" << std::endl
            << "Emulate: " << cpu.fname << std::endl;
  if(dump_points.use) std::cout << "- Dump: " << dump_points.fname << std::endl;
  if(intr_points.use) std::cout << "- Intr: " << dump_points.fname << std::endl;
  std::cout << "------------------------------------" << std::endl;

  // Run Emulator
  for(int t = 0;; ++t) {
    uint16_t pc = cpu.pc;
    uint32_t code = cpu.rom.at(pc);

    // Execute
    cpu.execute(code);
    cpu.mem.at(ZERO) = 0;
    int sout = cpu.serial();
    bool exit = cpu.cstop();

    // Print Serial Out
    if(sout != -1) std::cout << (cout_single_line ? "> " : "") << (char)sout << (cout_single_line ? "\n" : "");

    // Interrupt
    if(intr_points.contain(t)) cpu.interrupt(intr_points.at(t).ino);

    // Debug
    if(print_code) std::cout << "[" << hex(false, (uint16_t)t) << "]  " << cprint(hex(false, pc), GREEN, 0) << Debug::print_code(code) << std::endl;
    if(dump_all) std::cout << Debug::dump(cpu);
    if(dump_points.contain(pc)) std::cout << Debug::dump(cpu, dump_points.at(pc));
    if(interval_time) usleep(100000);

    // Exit
    if(exit) break;
  }
  std::cout << std::endl
            << "-----------------------------------#" << std::endl;
  return 0;
}
