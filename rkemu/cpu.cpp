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

void CPU::external_interrupt(int intr_no) {
  intr_latch[intr_no] = true;
}

void CPU::catch_interrupt() {
  if(mem.at(CSR) & IEN) {  // 割り込み許可か
    if(intr_latch[0]) mem.at(CSR) |= INTR0;
    if(intr_latch[1]) mem.at(CSR) |= INTR1;
    if(intr_latch[2]) mem.at(CSR) |= INTR2;
    if(intr_latch[3]) mem.at(CSR) |= INTR3;
  }
}

void CPU::jump_interrupt() {
  if(mem.at(CSR) & IEN) {  // 割り込み許可か
    if(mem.at(CSR) & INTR0) {
      intr_latch[0] = false;
      mem.at(IRA) = pc;
      pc = PC_INTR;
    }
    if(mem.at(CSR) & INTR1) {
      intr_latch[1] = false;
      mem.at(IRA) = pc;
      pc = PC_INTR;
    }
    if(mem.at(CSR) & INTR2) {
      intr_latch[2] = false;
      mem.at(IRA) = pc;
      pc = PC_INTR;
    }
    if(mem.at(CSR) & INTR3) {
      intr_latch[3] = false;
      mem.at(IRA) = pc;
      pc = PC_INTR;
    }
  }
}

void CPU::execute(uint32_t code) {
  OPDecoder op(code);
  if(op.opc == CALC) {
    if(op.func == ADD) add(op.rd, op.rs1, op.rs2);
    if(op.func == SUB) sub(op.rd, op.rs1, op.rs2);
    if(op.func == AND) land(op.rd, op.rs1, op.rs2);
    if(op.func == OR) lor(op.rd, op.rs1, op.rs2);
    if(op.func == XOR) lxor(op.rd, op.rs1, op.rs2);
    if(op.func == NOT) lnot(op.rd, op.rs1);
    if(op.func == LROT) lrot(op.rd, op.rs1);
    if(op.func == RROT) rrot(op.rd, op.rs1);
  }
  if(op.opc == CALCI) {
    if(op.func == ADD) addi(op.rd, op.rs1, op.imm);
    if(op.func == SUB) subi(op.rd, op.rs1, op.imm);
    if(op.func == AND) landi(op.rd, op.rs1, op.imm);
    if(op.func == OR) lori(op.rd, op.rs1, op.imm);
    if(op.func == XOR) lxori(op.rd, op.rs1, op.imm);
  }
  if(op.opc == LOAD) load(op.rd, op.rs1, op.imm);
  if(op.opc == LOADI) loadi(op.rd, op.imm);
  if(op.opc == STORE) store(op.rs1, op.rs2, op.imm);
  if(op.opc == JUMP) jump(op.rd, op.rs1, op.imm);
  if(op.opc == BREQ) breq(op.rs1, op.rs2, op.imm);
  if(op.opc == BRLT) brlt(op.rs1, op.rs2, op.imm);
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
