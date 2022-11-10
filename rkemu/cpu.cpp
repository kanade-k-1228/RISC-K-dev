#include "cpu.hpp"
#include "../rkisa/rkisa.hpp"
#include "utils.hpp"
#include <bitset>
#include <iomanip>
#include <sstream>

uint16_t decode_imm(uint32_t, uint16_t);
uint16_t decode_func(uint32_t, uint16_t);

CPU::CPU() : mem{0}, rom{0} {}

void CPU::load_rom(std::string fname) {
  std::string line;
  std::ifstream f(fname);
  if(!f) error("Cant Open File: " + fname);
  uint32_t buf;
  int op_cnt = 0;
  while(!f.eof()) {
    f.read((char*)&buf, sizeof(uint32_t));
    // std::cout << std::bitset<32>(buf) << std::endl;
    rom.at(op_cnt) = buf;
    op_cnt++;
  }
}

void CPU::dump() {
  std::cout << std::endl
            << "         | Save | Temp | Argu" << std::endl
            << "PC: " << hex(pc) << " | " << hex(mem.at(S0)) << " | " << hex(mem.at(T0)) << " | " << hex(mem.at(A0)) << std::endl
            << "RA: " << hex(mem.at(RA)) << " | " << hex(mem.at(S1)) << " | " << hex(mem.at(T1)) << " | " << hex(mem.at(A1)) << std::endl
            << "SP: " << hex(mem.at(SP)) << " | " << hex(mem.at(S2)) << " | " << hex(mem.at(T2)) << " | " << hex(mem.at(A2)) << std::endl
            << "GP: " << hex(mem.at(GP)) << " | " << hex(mem.at(S3)) << " | " << hex(mem.at(T3)) << " | " << hex(mem.at(A3)) << std::endl
            << std::endl;
}

void CPU::step() {
  uint32_t code = rom.at(pc);
  // std::cout << std::bitset<32>(code) << std::endl;

  uint16_t opc = (code >> 6) & 0x000f;
  uint16_t func = decode_func(code, opc);
  uint16_t rs1 = (code >> 0) & 0x003f;
  uint16_t rs2 = (code >> 20) & 0x003f;
  uint16_t rd = (code >> 26) & 0x003f;
  uint16_t imm = decode_imm(code, opc);

  if(opc == CALC) {
    if(func == ADD) add(rd, rs1, rs2);
    if(func == SUB) sub(rd, rs1, rs2);
  }
  if(opc == CALCI) {
    if(func == ADD) addi(rd, rs1, imm);
    if(func == SUB) subi(rd, rs1, imm);
  }
  if(opc == LOAD) load(rd, rs1, imm);
  if(opc == LOADI) loadi(rd, imm);
  if(opc == STORE) store(rs1, rs2, imm);
  if(opc == JUMP) jump(rd, rs1, imm);
  if(opc == BREQ) breq(rs1, rs2, imm);
  if(opc == BRLT) brlt(rs1, rs2, imm);
  mem.at(ZERO) = 0;

  return;
}

// add rd rs1 rs2
// rd = rs1 + rs2
void CPU::add(uint16_t rd, uint16_t rs1, uint16_t rs2) {
  std::cout << "add " << hex(rd) << " " << hex(rs1) << " " << hex(rs2) << std::endl;
  mem.at(rd) = mem.at(rs1) + mem.at(rs2);
  ++pc;
}

// addi rd rs1 imm
// rd = rs1 + imm
void CPU::addi(uint16_t rd, uint16_t rs1, uint16_t imm) {
  std::cout << "addi " << hex(rd) << " " << hex(rs1) << " " << hex(imm) << std::endl;
  mem.at(rd) = mem.at(rs1) + imm;
  ++pc;
}

// sub rd rs1 rs2
// rd = rs1 - rs2
void CPU::sub(uint16_t rd, uint16_t rs1, uint16_t rs2) {
  std::cout << "sub " << hex(rd) << " " << hex(rs1) << " " << hex(rs2) << std::endl;
  mem.at(rd) = mem.at(rs1) - mem.at(rs2);
  ++pc;
}

// subi rd rs1 imm
// rd = rs1 - imm
void CPU::subi(uint16_t rd, uint16_t rs1, uint16_t imm) {
  std::cout << "subi " << hex(rd) << " " << hex(rs1) << " " << hex(imm) << std::endl;
  mem.at(rd) = mem.at(rs1) - imm;
  ++pc;
}

// load rd rs1 imm
// rd <= m[rs1+imm]
void CPU::load(uint16_t rd, uint16_t rs1, uint16_t imm) {
  std::cout << "load " << hex(rd) << " " << hex(rs1) << " " << hex(imm) << std::endl;
  mem.at(rd) = mem.at(mem.at(rs1) + imm);
  ++pc;
}

// loadi rd imm
// rd <= imm
void CPU::loadi(uint16_t rd, uint16_t imm) {
  std::cout << "loadi " << hex(rd) << " " << hex(imm) << std::endl;
  mem.at(rd) = imm;
  ++pc;
}

// store rs1 rs2 imm
// m[rs1 + imm] <= rs2
void CPU::store(uint16_t rs1, uint16_t rs2, uint16_t imm) {
  std::cout << "store " << hex(rs1) << " " << hex(imm) << std::endl;
  mem.at(mem.at(rs1) + imm) = mem.at(rs2);
  ++pc;
}

// jump rd rs1 imm
// rd <= pc + 1
// pc <= rs1 + imm
void CPU::jump(uint16_t rd, uint16_t rs1, uint16_t imm) {
  std::cout << "jump " << hex(rd) << " " << hex(rs1) << " " << hex(imm) << std::endl;
  mem.at(rd) = pc + 1;
  pc = mem.at(rs1) + imm;
}

// breq rs1 rs2 imm
// if(rs1==rs2) pc <= imm
void CPU::breq(uint16_t rs1, uint16_t rs2, uint16_t imm) {
  std::cout << "breq " << hex(rs1) << " " << hex(rs2) << " " << hex(imm) << std::endl;
  pc = (mem.at(rs1) == mem.at(rs2)) ? imm : pc + 1;
}

// brlt rs1 rs2 imm
// if(rs1<rs2) pc <= imm
void CPU::brlt(uint16_t rs1, uint16_t rs2, uint16_t imm) {
  std::cout << "brlt " << hex(rs1) << " " << hex(rs2) << " " << hex(imm) << std::endl;
  pc = (mem.at(rs1) < mem.at(rs2)) ? imm : pc + 1;
}

uint16_t decode_imm(uint32_t code, uint16_t opc) {
  if(opc == CALC) return 0;
  if(opc == CALCI) return ((code >> 10) & 0x03ff) | (((code >> 24) & 0x0003) << 10);
  if(opc == LOAD || opc == LOADI || opc == JUMP) return (code >> 10) & 0xffff;
  if(opc == STORE || opc == BREQ || opc == BRLT) return ((code >> 10) & 0x03ff) | ((code >> 26) & 0x003f);
  error("Unknown OPC: " + opc);
  return 0;
}

uint16_t decode_func(uint32_t code, uint16_t opc) {
  if(opc == CALC) return (code >> 10) & 0x000f;
  if(opc == CALCI) return (code >> 20) & 0x000f;
  return 0;
}