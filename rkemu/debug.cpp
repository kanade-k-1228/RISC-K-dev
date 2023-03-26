#include "debug.hpp"
#include "../rkisa/rkisa.hpp"
#include "../utils/utils.hpp"
#include <fstream>
#include <regex>
#include <string>

void IntrPoints::init(std::string fname) {
  this->fname = fname;
  this->use = true;
  std::ifstream fin(fname);
  if(!fin) error("Cant Open File: " + fname);
  for(std::string line; std::getline(fin, line);) {
    auto tokens = split(line, ' ');
    IntrOption intr_opt;
    int t = std::stoi(tokens.at(0), nullptr, 0);
    intr_opt.ino = std::stoi(tokens.at(1));
    this->insert(std::make_pair(t, intr_opt));
  }
}

bool IntrPoints::is_intr(uint16_t time) {
  if(use)
    return this->contains(time);
  else
    return false;
}

void DumpPoints::init(std::string fname) {
  this->fname = fname;
  this->use = true;
  std::ifstream fin(fname);
  if(!fin) error("Cant Open File: " + fname);
  for(std::string line; std::getline(fin, line);) {
    auto tokens = split(line, ' ');
    if(tokens.size() == 0) continue;
    DumpOption dump;
    uint16_t pc = std::stoi(tokens.at(0), nullptr, 0);
    for(unsigned int i = 1; i < tokens.size(); ++i) {
      std::string token = tokens.at(i);
      if(token == "-s") {
        dump.stack = true;
      } else if(token == "-") {
        uint16_t begin = std::stoi(tokens.at(i - 1), nullptr, 0);
        uint16_t end = std::stoi(tokens.at(i + 1), nullptr, 0);
        for(uint16_t j = begin; j < end; ++j) dump.address.insert(j);
        ++i;
      } else {
        dump.address.insert(std::stoi(token, nullptr, 0));
      }
    }
    this->insert(std::make_pair(pc, dump));
  }
}

bool DumpPoints::is_dump(uint16_t pc) {
  if(all)
    return true;
  else if(use)
    return this->contains(pc);
  return false;
}

std::string Debug::print_code(uint32_t code) {
  OPDecoder op(code);
  if(op.opc == CALC) {
    if(op.func == ADD) return cprint("add", RED, 6) + cprint(hex(false, op.rd), BLUE, 6) + cprint(hex(false, op.rs1), BLUE, 6) + cprint(hex(false, op.rs2), BLUE, 6);
    if(op.func == SUB) return cprint("sub", RED, 6) + cprint(hex(false, op.rd), BLUE, 6) + cprint(hex(false, op.rs1), BLUE, 6) + cprint(hex(false, op.rs2), BLUE, 6);
    if(op.func == AND) return cprint("and", RED, 6) + cprint(hex(false, op.rd), BLUE, 6) + cprint(hex(false, op.rs1), BLUE, 6) + cprint(hex(false, op.rs2), BLUE, 6);
    if(op.func == OR) return cprint("or", RED, 6) + cprint(hex(false, op.rd), BLUE, 6) + cprint(hex(false, op.rs1), BLUE, 6) + cprint(hex(false, op.rs2), BLUE, 6);
    if(op.func == XOR) return cprint("xor", RED, 6) + cprint(hex(false, op.rd), BLUE, 6) + cprint(hex(false, op.rs1), BLUE, 6) + cprint(hex(false, op.rs2), BLUE, 6);
    if(op.func == NOT) return cprint("not", RED, 6) + cprint(hex(false, op.rd), BLUE, 6) + cprint(hex(false, op.rs1), BLUE, 6);
    if(op.func == LROT) return cprint("lrot", RED, 6) + cprint(hex(false, op.rd), BLUE, 6) + cprint(hex(false, op.rs1), BLUE, 6);
    if(op.func == RROT) return cprint("rrot", RED, 6) + cprint(hex(false, op.rd), BLUE, 6) + cprint(hex(false, op.rs1), BLUE, 6);
  }
  if(op.opc == CALCI) {
    if(op.func == ADD) return cprint("addi", RED, 6) + cprint(hex(false, op.rd), BLUE, 6) + cprint(hex(false, op.rs1), BLUE, 6) + cprint(hex(false, op.imm), YELLOW, 6);
    if(op.func == SUB) return cprint("subi", RED, 6) + cprint(hex(false, op.rd), BLUE, 6) + cprint(hex(false, op.rs1), BLUE, 6) + cprint(hex(false, op.imm), YELLOW, 6);
    if(op.func == AND) return cprint("andi", RED, 6) + cprint(hex(false, op.rd), BLUE, 6) + cprint(hex(false, op.rs1), BLUE, 6) + cprint(hex(false, op.imm), YELLOW, 6);
    if(op.func == OR) return cprint("ori", RED, 6) + cprint(hex(false, op.rd), BLUE, 6) + cprint(hex(false, op.rs1), BLUE, 6) + cprint(hex(false, op.imm), YELLOW, 6);
    if(op.func == XOR) return cprint("xori", RED, 6) + cprint(hex(false, op.rd), BLUE, 6) + cprint(hex(false, op.rs1), BLUE, 6) + cprint(hex(false, op.imm), YELLOW, 6);
  }
  if(op.opc == LOAD) return cprint("load", RED, 6) + cprint(hex(false, op.rd), BLUE, 6) + cprint(hex(false, op.rs1), BLUE, 6) + cprint(hex(false, op.imm), YELLOW, 6);
  if(op.opc == LOADI) return cprint("loadi", RED, 6) + cprint(hex(false, op.rd), BLUE, 6) + cprint(hex(false, op.imm), YELLOW, 6);
  if(op.opc == STORE) return cprint("store", RED, 6) + cprint(hex(false, op.rs2), BLUE, 6) + cprint(hex(false, op.rs1), BLUE, 6) + cprint(hex(false, op.imm), YELLOW, 6);
  if(op.opc == JUMP) return cprint("jump", RED, 6) + cprint(hex(false, op.rd), BLUE, 6) + cprint(hex(false, op.rs1), BLUE, 6) + cprint(hex(false, op.imm), YELLOW, 6);
  if(op.opc == BREQ) return cprint("breq", RED, 6) + cprint(hex(false, op.rs1), BLUE, 6) + cprint(hex(false, op.rs2), BLUE, 6) + cprint(hex(false, op.imm), YELLOW, 6);
  if(op.opc == BRLT) return cprint("brlt", RED, 6) + cprint(hex(false, op.rs1), BLUE, 6) + cprint(hex(false, op.rs2), BLUE, 6) + cprint(hex(false, op.imm), YELLOW, 6);
  return "Unknown Opecode" + hex(true, op.opc);
}

std::string Debug::dump(CPU& cpu, DumpOption& opt) {
  std::stringstream ss;
  ss << " +-----------+------+------+------+" << std::endl
     << " |  " << cprint("PC: " + hex(false, cpu.pc), GREEN, 0) << " | " << cprint("Save", BLUE, 0) << " | " << cprint("Temp", BLUE, 0) << " | " << cprint("Arg", BLUE, 0) << "  |" << std::endl
     << " |  RA: " << hex(false, cpu.mem.at(RA)) << " | " << hex(false, cpu.mem.at(S0)) << " | " << hex(false, cpu.mem.at(T0)) << " | " << hex(false, cpu.mem.at(A0)) << " |" << std::endl
     << " | IRA: " << hex(false, cpu.mem.at(IRA)) << " | " << hex(false, cpu.mem.at(S1)) << " | " << hex(false, cpu.mem.at(T1)) << " | " << hex(false, cpu.mem.at(A1)) << " |" << std::endl
     << " |  SP: " << hex(false, cpu.mem.at(SP)) << " | " << hex(false, cpu.mem.at(S2)) << " | " << hex(false, cpu.mem.at(T2)) << " | " << hex(false, cpu.mem.at(A2)) << " |" << std::endl
     << " |  FP: " << hex(false, cpu.mem.at(FP)) << " | " << hex(false, cpu.mem.at(S3)) << " | " << hex(false, cpu.mem.at(T3)) << " | " << hex(false, cpu.mem.at(A3)) << " |" << std::endl
     << " +-----------+------+------+------+" << std::endl
     << " | CSR: " << std::bitset<16>(cpu.mem.at(CSR)) << "          |" << std::endl;
  if(opt.address.size() != 0) ss << " +--------------------------------+" << std::endl;
  for(uint16_t addr : opt.address)
    ss << " | " << hex(true, addr) << " : " << hex(false, cpu.mem.at(addr)) << "                  |" << std::endl;
  if(opt.stack) {
    if(cpu.mem.at(SP) < cpu.mem.at(FP)) ss << " +--------------------------------+" << std::endl;
    for(uint16_t sp = cpu.mem.at(SP); sp < cpu.mem.at(FP); sp++)
      ss << " | " << hex(true, (uint16_t)(sp + 1)) << " : " << hex(false, cpu.mem.at(sp + 1)) << "                  |" << std::endl;
  }
  ss << " +--------------------------------+" << std::endl;
  return ss.str();
}

std::string Debug::dump(CPU& cpu) {
  std::stringstream ss;
  ss << " +-----------+------+------+------+" << std::endl
     << " |  " << cprint("PC: " + hex(false, cpu.pc), GREEN, 0) << " | " << cprint("Save", BLUE, 0) << " | " << cprint("Temp", BLUE, 0) << " | " << cprint("Arg", BLUE, 0) << "  |" << std::endl
     << " |  RA: " << hex(false, cpu.mem.at(RA)) << " | " << hex(false, cpu.mem.at(S0)) << " | " << hex(false, cpu.mem.at(T0)) << " | " << hex(false, cpu.mem.at(A0)) << " |" << std::endl
     << " | IRA: " << hex(false, cpu.mem.at(IRA)) << " | " << hex(false, cpu.mem.at(S1)) << " | " << hex(false, cpu.mem.at(T1)) << " | " << hex(false, cpu.mem.at(A1)) << " |" << std::endl
     << " |  SP: " << hex(false, cpu.mem.at(SP)) << " | " << hex(false, cpu.mem.at(S2)) << " | " << hex(false, cpu.mem.at(T2)) << " | " << hex(false, cpu.mem.at(A2)) << " |" << std::endl
     << " |  FP: " << hex(false, cpu.mem.at(FP)) << " | " << hex(false, cpu.mem.at(S3)) << " | " << hex(false, cpu.mem.at(T3)) << " | " << hex(false, cpu.mem.at(A3)) << " |" << std::endl
     << " +-----------+------+------+------+" << std::endl
     << " | CSR: " << std::bitset<16>(cpu.mem.at(CSR)) << "          |" << std::endl
     << " +--------------------------------+" << std::endl;
  return ss.str();
}
