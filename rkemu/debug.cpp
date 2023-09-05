#include "debug.hpp"
#include "../rkisa/isa.hpp"
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

std::string Debug::print_code(uint32_t code) {
  OPDecoder op(code);
  if(op.opc == OPCode::calc) {
    if(op.func == ALUCode::ADD) return red("add") + blue(hex(op.rd)) + blue(hex(op.rs1)) + blue(hex(op.rs2));
    if(op.func == ALUCode::SUB) return red("sub") + blue(hex(op.rd)) + blue(hex(op.rs1)) + blue(hex(op.rs2));
    if(op.func == ALUCode::AND) return red("and") + blue(hex(op.rd)) + blue(hex(op.rs1)) + blue(hex(op.rs2));
    if(op.func == ALUCode::OR) return red("or") + blue(hex(op.rd)) + blue(hex(op.rs1)) + blue(hex(op.rs2));
    if(op.func == ALUCode::XOR) return red("xor") + blue(hex(op.rd)) + blue(hex(op.rs1)) + blue(hex(op.rs2));
    if(op.func == ALUCode::NOT) return red("not") + blue(hex(op.rd)) + blue(hex(op.rs1));
    if(op.func == ALUCode::SRS) return red("srs") + blue(hex(op.rd)) + blue(hex(op.rs1));
    if(op.func == ALUCode::SRU) return red("sru") + blue(hex(op.rd)) + blue(hex(op.rs1));
    if(op.func == ALUCode::SL) return red("sl") + blue(hex(op.rd)) + blue(hex(op.rs1));
    if(op.func == ALUCode::EQ) return red("eq") + blue(hex(op.rd)) + blue(hex(op.rs1));
    if(op.func == ALUCode::LTS) return red("lts") + blue(hex(op.rd)) + blue(hex(op.rs1));
    if(op.func == ALUCode::LTU) return red("ltu") + blue(hex(op.rd)) + blue(hex(op.rs1));
    throw new std::string("Unknown FuncCode" + hex(op.func));
  }
  if(op.opc == OPCode::calci) {
    if(op.func == ALUCode::ADD) return red("addi") + blue(hex(op.rd)) + blue(hex(op.rs1)) + yellow(hex(op.imm));
    if(op.func == ALUCode::SUB) return red("subi") + blue(hex(op.rd)) + blue(hex(op.rs1)) + yellow(hex(op.imm));
    if(op.func == ALUCode::AND) return red("andi") + blue(hex(op.rd)) + blue(hex(op.rs1)) + yellow(hex(op.imm));
    if(op.func == ALUCode::OR) return red("ori") + blue(hex(op.rd)) + blue(hex(op.rs1)) + yellow(hex(op.imm));
    if(op.func == ALUCode::XOR) return red("xori") + blue(hex(op.rd)) + blue(hex(op.rs1)) + yellow(hex(op.imm));
    if(op.func == ALUCode::EQ) return red("eqi") + blue(hex(op.rd)) + blue(hex(op.rs1)) + yellow(hex(op.imm));
    if(op.func == ALUCode::LTS) return red("ltsi") + blue(hex(op.rd)) + blue(hex(op.rs1)) + yellow(hex(op.imm));
    if(op.func == ALUCode::LTU) return red("ltui") + blue(hex(op.rd)) + blue(hex(op.rs1)) + yellow(hex(op.imm));
    throw new std::string("Unknown FuncCode" + hex(op.func));
  }
  if(op.opc == OPCode::load) return red("load") + blue(hex(op.rd)) + blue(hex(op.rs1)) + yellow(hex(op.imm));
  if(op.opc == OPCode::store) return red("store") + blue(hex(op.rs2)) + blue(hex(op.rs1)) + yellow(hex(op.imm));
  if(op.opc == OPCode::ctrl) return red("ctrl") + blue(hex(op.rd)) + blue(hex(op.rs2)) + blue(hex(op.rs1)) + yellow(hex(op.imm));
  throw new std::string("Unknown Opecode" + hex(op.opc));
}

std::string Debug::dump(CPU& cpu, DumpOption& opt) {
  std::stringstream ss;
  ss << " +------------+-------------------------+" << std::endl
     << " |  " << green("PC: " + hex(cpu.ram.get(Reg::pc))) << "  |" << std::endl
     << " |  RA: " << hex(cpu.ram.get(Reg::ra)) << "  |  S0: " << hex(cpu.ram.get(Reg::s0)) << "  |  T0: " << hex(cpu.ram.get(Reg::t0)) << "  |" << std::endl
     << " | IRA: " << hex(cpu.ram.get(Reg::ira)) << "  |  S1: " << hex(cpu.ram.get(Reg::s1)) << "  |  T1: " << hex(cpu.ram.get(Reg::t1)) << "  |" << std::endl
     << " |  SP: " << hex(cpu.ram.get(Reg::sp)) << "  |  S2: " << hex(cpu.ram.get(Reg::s2)) << "  |  T2: " << hex(cpu.ram.get(Reg::t2)) << "  |" << std::endl
     << " |  FP: " << hex(cpu.ram.get(Reg::fp)) << "  |  S3: " << hex(cpu.ram.get(Reg::s3)) << "  |  T3: " << hex(cpu.ram.get(Reg::t3)) << "  |" << std::endl
     << " +------------+------------+------------+" << std::endl;
  for(uint16_t addr : opt.address)
    ss << " | " << hex(addr) << " : " << hex(cpu.ram.get(addr)) << "                  |" << std::endl;
  if(opt.stack) {
    if(cpu.ram.get(Reg::sp) < cpu.ram.get(Reg::fp)) ss << " +--------------------------------+" << std::endl;
    for(uint16_t sp = cpu.ram.get(Reg::sp); sp < cpu.ram.get(Reg::fp); sp++)
      ss << " | " << hex((uint16_t)(sp + 1)) << " : " << hex(cpu.ram.get(sp + 1)) << "                  |" << std::endl;
  }
  return ss.str();
}

std::string Debug::dump(CPU& cpu) {
  std::stringstream ss;
  ss << " +------------+-------------------------+" << std::endl
     << " |  " << green("PC: " + hex(cpu.ram.get(Reg::pc))) << "  |" << std::endl
     << " |  RA: " << hex(cpu.ram.get(Reg::ra)) << "  |  S0: " << hex(cpu.ram.get(Reg::s0)) << "  |  T0: " << hex(cpu.ram.get(Reg::t0)) << "  |" << std::endl
     << " | IRA: " << hex(cpu.ram.get(Reg::ira)) << "  |  S1: " << hex(cpu.ram.get(Reg::s1)) << "  |  T1: " << hex(cpu.ram.get(Reg::t1)) << "  |" << std::endl
     << " |  SP: " << hex(cpu.ram.get(Reg::sp)) << "  |  S2: " << hex(cpu.ram.get(Reg::s2)) << "  |  T2: " << hex(cpu.ram.get(Reg::t2)) << "  |" << std::endl
     << " |  FP: " << hex(cpu.ram.get(Reg::fp)) << "  |  S3: " << hex(cpu.ram.get(Reg::s3)) << "  |  T3: " << hex(cpu.ram.get(Reg::t3)) << "  |" << std::endl
     << " +------------+------------+------------+" << std::endl;
  return ss.str();
}
