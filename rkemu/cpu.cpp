#include "cpu.hpp"
#include "../rkisa/rkisa.hpp"
#include "../utils/utils.hpp"
#include <bitset>
#include <iomanip>
#include <sstream>

uint16_t decode_imm(uint32_t, uint16_t);
uint16_t decode_func(uint32_t, uint16_t);

CPU::CPU() : mem{0}, rom{0} {
  mem.at(SP) = 0xffff;
  mem.at(FP) = 0xffff;
}

void CPU::load_rom(std::string fname) {
  this->fname = fname;
  std::ifstream f(fname);
  if(!f) error("Cant Open File: " + fname);
  uint32_t buf;
  for(int op_cnt = 0; !f.eof(); op_cnt++) {
    f.read((char*)&buf, sizeof(uint32_t));
    rom.at(op_cnt) = buf;
  }
}

bool CPU::cstop() {
  return mem.at(CSR) & CSRBit::CSTOP;
}

int CPU::serial() {
  char cout = mem.at(COUT);
  if(cout) {
    mem.at(COUT) = 0;
    return cout;
  } else {
    return -1;
  }
}

void CPU::interrupt(int intr_no) {
  if(mem.at(CSR) & IEN) {
    if(intr_no == 0) mem.at(CSR) |= INTR0;
    if(intr_no == 1) mem.at(CSR) |= INTR1;
    if(intr_no == 2) mem.at(CSR) |= INTR2;
    if(intr_no == 3) mem.at(CSR) |= INTR3;
    mem.at(IRA) = pc;
    pc = PC_INTR;
  }
}

void CPU::execute(uint32_t code) {
  uint16_t opc = (code >> 6) & 0x000f;
  uint16_t func = decode_func(code, opc);
  uint16_t rs1 = (code >> 0) & 0x003f;
  uint16_t rs2 = (code >> 20) & 0x003f;
  uint16_t rd = (code >> 26) & 0x003f;
  uint16_t imm = decode_imm(code, opc);

  if(opc == CALC) {
    if(func == ADD) add(rd, rs1, rs2);
    if(func == SUB) sub(rd, rs1, rs2);
    if(func == AND) land(rd, rs1, rs2);
    if(func == OR) lor(rd, rs1, rs2);
    if(func == XOR) lxor(rd, rs1, rs2);
    if(func == NOT) lnot(rd, rs1);
    if(func == LROT) lrot(rd, rs1);
    if(func == RROT) rrot(rd, rs1);
  }
  if(opc == CALCI) {
    if(func == ADD) addi(rd, rs1, imm);
    if(func == SUB) subi(rd, rs1, imm);
    if(func == AND) landi(rd, rs1, imm);
    if(func == OR) lori(rd, rs1, imm);
    if(func == XOR) lxori(rd, rs1, imm);
  }
  if(opc == LOAD) load(rd, rs1, imm);
  if(opc == LOADI) loadi(rd, imm);
  if(opc == STORE) store(rs1, rs2, imm);
  if(opc == JUMP) jump(rd, rs1, imm);
  if(opc == BREQ) breq(rs1, rs2, imm);
  if(opc == BRLT) brlt(rs1, rs2, imm);
  return;
}

// add rd rs1 rs2
// rd = rs1 + rs2
void CPU::add(uint16_t rd, uint16_t rs1, uint16_t rs2) {
  mem.at(rd) = mem.at(rs1) + mem.at(rs2);
  ++pc;
}

// addi rd rs1 imm
// rd = rs1 + imm
void CPU::addi(uint16_t rd, uint16_t rs1, uint16_t imm) {
  mem.at(rd) = mem.at(rs1) + imm;
  ++pc;
}

// sub rd rs1 rs2
// rd = rs1 - rs2
void CPU::sub(uint16_t rd, uint16_t rs1, uint16_t rs2) {
  mem.at(rd) = mem.at(rs1) - mem.at(rs2);
  ++pc;
}

// subi rd rs1 imm
// rd = rs1 - imm
void CPU::subi(uint16_t rd, uint16_t rs1, uint16_t imm) {
  mem.at(rd) = mem.at(rs1) - imm;
  ++pc;
}

// and rd rs1 rs2
// rd = rs1 & rs2
void CPU::land(uint16_t rd, uint16_t rs1, uint16_t rs2) {
  mem.at(rd) = mem.at(rs1) & mem.at(rs2);
  ++pc;
}

// andi rd rs1 imm
// rd = rs1 & imm
void CPU::landi(uint16_t rd, uint16_t rs1, uint16_t imm) {
  mem.at(rd) = mem.at(rs1) & imm;
  ++pc;
}

// or rd rs1 rs2
// rd = rs1 | rs2
void CPU::lor(uint16_t rd, uint16_t rs1, uint16_t rs2) {
  mem.at(rd) = mem.at(rs1) | mem.at(rs2);
  ++pc;
}

// ori rd rs1 imm
// rd = rs1 | imm
void CPU::lori(uint16_t rd, uint16_t rs1, uint16_t imm) {
  mem.at(rd) = mem.at(rs1) | imm;
  ++pc;
}

// xor rd rs1 rs2
// rd = rs1 ^ rs2
void CPU::lxor(uint16_t rd, uint16_t rs1, uint16_t rs2) {
  mem.at(rd) = mem.at(rs1) ^ mem.at(rs2);
  ++pc;
}

// xori rd rs1 imm
// rd = rs1 ^ imm
void CPU::lxori(uint16_t rd, uint16_t rs1, uint16_t imm) {
  mem.at(rd) = mem.at(rs1) ^ imm;
  ++pc;
}

// not rd rs1
// rd = ~rs1
void CPU::lnot(uint16_t rd, uint16_t rs1) {
  mem.at(rd) = ~mem.at(rs1);
  ++pc;
}

// lrot rd rs1
// rd = <rs1<
void CPU::lrot(uint16_t rd, uint16_t rs1) {
  mem.at(rd) = mem.at(rs1) << 1 | mem.at(rs1) >> 15;
  ++pc;
}

// rrot rd rs1
// rd = >rs1>
void CPU::rrot(uint16_t rd, uint16_t rs1) {
  mem.at(rd) = mem.at(rs1) >> 1 | mem.at(rs1) << 15;
  ++pc;
}

// load rd rs1 imm
// rd = m[rs1+imm]
void CPU::load(uint16_t rd, uint16_t rs1, uint16_t imm) {
  mem.at(rd) = mem.at(mem.at(rs1) + imm);
  ++pc;
}

// loadi rd imm
// rd = imm
void CPU::loadi(uint16_t rd, uint16_t imm) {
  mem.at(rd) = imm;
  ++pc;
}

// store rs1 rs2 imm
// m[rs1 + imm] = rs2
void CPU::store(uint16_t rs1, uint16_t rs2, uint16_t imm) {
  mem.at(mem.at(rs1) + imm) = mem.at(rs2);
  ++pc;
}

// jump rd rs1 imm
// rd = pc + 1
// pc = rs1 + imm
void CPU::jump(uint16_t rd, uint16_t rs1, uint16_t imm) {
  mem.at(rd) = pc + 1;
  mem.at(ZERO) = 0x0000;
  pc = mem.at(rs1) + imm;
}

// breq rs1 rs2 imm
// if(rs1==rs2) pc = imm
void CPU::breq(uint16_t rs1, uint16_t rs2, uint16_t imm) {
  pc = (mem.at(rs1) == mem.at(rs2)) ? imm : pc + 1;
}

// brlt rs1 rs2 imm
// if(rs1<rs2) pc = imm
void CPU::brlt(uint16_t rs1, uint16_t rs2, uint16_t imm) {
  pc = (mem.at(rs1) < mem.at(rs2)) ? imm : pc + 1;
}
