#include "debug.hpp"
#include "../rkarch/arch.hpp"
#include "model.hpp"
#include "utils.hpp"
#include <fstream>
#include <regex>
#include <string>

void IntrPoints::init(std::string fname) {
  this->fname = fname;
  this->use = true;
  std::ifstream fin(fname);
  if(!fin) throw new std::string("Cant Open File: " + fname);
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
  if(!fin) throw new std::string("Cant Open File: " + fname);
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

std::string reg(uint8_t reg) {
  if(reg == Reg::zero) return "zero";
  if(reg == Reg::ira) return "ira";
  if(reg == Reg::pc) return "pc";
  if(reg == Reg::sp) return "sp";
  if(reg == Reg::ra) return "ra";
  if(reg == Reg::fp) return "fp";
  if(reg == Reg::a0) return "a0";
  if(reg == Reg::a1) return "a1";
  if(reg == Reg::t0) return "t0";
  if(reg == Reg::t1) return "t1";
  if(reg == Reg::t2) return "t2";
  if(reg == Reg::t3) return "t3";
  if(reg == Reg::s0) return "s0";
  if(reg == Reg::s1) return "s1";
  if(reg == Reg::s2) return "s2";
  if(reg == Reg::s3) return "s3";
  throw new std::string("Unknown registor code: " + std::to_string(reg));
}

std::string Debug::print_code(uint32_t code) {
  Decoder op(code);
  if(op.opc == OPCode::calc) {
    if(op.func == CalcFunc::ADD) return red(left("add", 6)) + blue(left(reg(op.rd), 6)) + blue(left(reg(op.rs1), 6)) + blue(left(reg(op.rs2), 6));
    if(op.func == CalcFunc::SUB) return red(left("sub", 6)) + blue(left(reg(op.rd), 6)) + blue(left(reg(op.rs1), 6)) + blue(left(reg(op.rs2), 6));
    if(op.func == CalcFunc::AND) return red(left("and", 6)) + blue(left(reg(op.rd), 6)) + blue(left(reg(op.rs1), 6)) + blue(left(reg(op.rs2), 6));
    if(op.func == CalcFunc::OR) return red(left("or", 6)) + blue(left(reg(op.rd), 6)) + blue(left(reg(op.rs1), 6)) + blue(left(reg(op.rs2), 6));
    if(op.func == CalcFunc::XOR) return red(left("xor", 6)) + blue(left(reg(op.rd), 6)) + blue(left(reg(op.rs1), 6)) + blue(left(reg(op.rs2), 6));
    if(op.func == CalcFunc::NOT) return red(left("not", 6)) + blue(left(reg(op.rd), 6)) + blue(left(reg(op.rs1), 6));
    if(op.func == CalcFunc::SRS) return red(left("srs", 6)) + blue(left(reg(op.rd), 6)) + blue(left(reg(op.rs1), 6));
    if(op.func == CalcFunc::SRU) return red(left("sru", 6)) + blue(left(reg(op.rd), 6)) + blue(left(reg(op.rs1), 6));
    if(op.func == CalcFunc::SL) return red(left("sl", 6)) + blue(left(reg(op.rd), 6)) + blue(left(reg(op.rs1), 6));
    if(op.func == CalcFunc::EQ) return red(left("eq", 6)) + blue(left(reg(op.rd), 6)) + blue(left(reg(op.rs1), 6));
    if(op.func == CalcFunc::LTS) return red(left("lts", 6)) + blue(left(reg(op.rd), 6)) + blue(left(reg(op.rs1), 6));
    if(op.func == CalcFunc::LTU) return red(left("ltu", 6)) + blue(left(reg(op.rd), 6)) + blue(left(reg(op.rs1), 6));
    throw new std::string("Unknown FuncCode" + hex(op.func));
  }
  if(op.opc == OPCode::calci) {
    if(op.func == CalcFunc::ADD) return red(left("addi", 6)) + blue(left(reg(op.rd), 6)) + blue(left(reg(op.rs1), 6)) + yellow(hex(op.imm));
    if(op.func == CalcFunc::SUB) return red(left("subi", 6)) + blue(left(reg(op.rd), 6)) + blue(left(reg(op.rs1), 6)) + yellow(hex(op.imm));
    if(op.func == CalcFunc::AND) return red(left("andi", 6)) + blue(left(reg(op.rd), 6)) + blue(left(reg(op.rs1), 6)) + yellow(hex(op.imm));
    if(op.func == CalcFunc::OR) return red(left("ori", 6)) + blue(left(reg(op.rd), 6)) + blue(left(reg(op.rs1), 6)) + yellow(hex(op.imm));
    if(op.func == CalcFunc::XOR) return red(left("xori", 6)) + blue(left(reg(op.rd), 6)) + blue(left(reg(op.rs1), 6)) + yellow(hex(op.imm));
    if(op.func == CalcFunc::EQ) return red(left("eqi", 6)) + blue(left(reg(op.rd), 6)) + blue(left(reg(op.rs1), 6)) + yellow(hex(op.imm));
    if(op.func == CalcFunc::LTS) return red(left("ltsi", 6)) + blue(left(reg(op.rd), 6)) + blue(left(reg(op.rs1), 6)) + yellow(hex(op.imm));
    if(op.func == CalcFunc::LTU) return red(left("ltui", 6)) + blue(left(reg(op.rd), 6)) + blue(left(reg(op.rs1), 6)) + yellow(hex(op.imm));
    throw new std::string("Unknown FuncCode" + hex(op.func));
  }
  if(op.opc == OPCode::load) return red(left("load", 6)) + blue(left(reg(op.rd), 6)) + blue(left(reg(op.rs1), 6)) + yellow(hex(op.imm));
  if(op.opc == OPCode::store) return red(left("store", 6)) + blue(left(reg(op.rs2), 6)) + blue(left(reg(op.rs1), 6)) + yellow(hex(op.imm));
  if(op.opc == OPCode::ctrl) return red(left("ctrl", 6)) + blue(left(reg(op.rd), 6)) + blue(left(reg(op.rs2), 6)) + blue(left(reg(op.rs1), 6)) + yellow(hex(op.imm));
  throw new std::string("Unknown Opecode" + hex(op.opc));
}

std::string Debug::dump(CPU& cpu, DumpOption& opt) {
  std::stringstream ss;
  ss << dump_reg(cpu);
  for(uint16_t addr : opt.address)
    ss << " | " << hex(addr) << " : " << hex(cpu.ram.get(addr)) << "                                 |" << std::endl;
  if(opt.stack) {
    for(uint16_t sp = cpu.ram.get(Reg::sp); sp < cpu.ram.get(Reg::fp); sp++)
      ss << " | " << hex((uint16_t)(sp + 1)) << " : " << hex(cpu.ram.get(sp + 1)) << "                                 |" << std::endl;
  }
  ss << " +---------------------------------------------+" << std::endl;
  return ss.str();
}

std::string Debug::dump_reg(CPU& cpu) {
  std::stringstream ss;
  ss << " +------------+----------+----------+----------+" << std::endl
     << " | zero: " << hex(cpu.ram.get(Reg::zero)) << " | ra: " << hex(cpu.ram.get(Reg::ra)) << " | t0: " << hex(cpu.ram.get(Reg::t0)) << " | s0: " << hex(cpu.ram.get(Reg::s0)) << " |" << std::endl
     << " |  ira: " << hex(cpu.ram.get(Reg::ira)) << " | fp: " << hex(cpu.ram.get(Reg::fp)) << " | t1: " << hex(cpu.ram.get(Reg::t1)) << " | s1: " << hex(cpu.ram.get(Reg::s1)) << " |" << std::endl
     << " |   pc: " << hex(cpu.ram.get(Reg::pc)) << " | a0: " << hex(cpu.ram.get(Reg::a0)) << " | t2: " << hex(cpu.ram.get(Reg::t2)) << " | s2: " << hex(cpu.ram.get(Reg::s2)) << " |" << std::endl
     << " |   sp: " << hex(cpu.ram.get(Reg::sp)) << " | a1: " << hex(cpu.ram.get(Reg::a1)) << " | t3: " << hex(cpu.ram.get(Reg::t3)) << " | s3: " << hex(cpu.ram.get(Reg::s3)) << " |" << std::endl
     << " +------------+----------+----------+----------+" << std::endl;
  return ss.str();
}
