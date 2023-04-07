#include "cpu.hpp"
#include "isa.hpp"
#include "utils.hpp"
#include <bitset>
#include <iomanip>
#include <sstream>

uint16_t decode_imm(uint32_t, uint16_t);
uint16_t decode_func(uint32_t, uint16_t);

CPU::CPU() : rom{0} {}

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
  return ram.get(Reg::csr) & CSRBit::CSTOP;
}

int CPU::serial() {
  char cout = ram.get(Reg::cout);
  if(cout) {
    ram.set(Reg::cout, 0);
    return cout;
  } else {
    return -1;
  }
}

void CPU::external_interrupt(int intr_no) {
  intr_latch[intr_no] = true;
}

void CPU::catch_interrupt() {
  if(ram.get(Reg::csr) & CSRBit::IEN) {  // 割り込み許可か
    if(intr_latch[0]) ram.set(Reg::csr, ram.get(Reg::csr) | CSRBit::INTR0);
    if(intr_latch[1]) ram.set(Reg::csr, ram.get(Reg::csr) | CSRBit::INTR1);
    if(intr_latch[2]) ram.set(Reg::csr, ram.get(Reg::csr) | CSRBit::INTR2);
    if(intr_latch[3]) ram.set(Reg::csr, ram.get(Reg::csr) | CSRBit::INTR3);
  }
}

void CPU::jump_interrupt() {
  if(ram.get(Reg::csr) & CSRBit::IEN) {  // 割り込み許可か
    if(ram.get(Reg::csr) & CSRBit::INTR0) intr_latch[0] = false, ram.set_ira(), ram.set_pc(Addr::PC_INTR);
    if(ram.get(Reg::csr) & CSRBit::INTR1) intr_latch[1] = false, ram.set_ira(), ram.set_pc(Addr::PC_INTR);
    if(ram.get(Reg::csr) & CSRBit::INTR2) intr_latch[2] = false, ram.set_ira(), ram.set_pc(Addr::PC_INTR);
    if(ram.get(Reg::csr) & CSRBit::INTR3) intr_latch[3] = false, ram.set_ira(), ram.set_pc(Addr::PC_INTR);
  }
}

void CPU::execute(uint32_t code) {
  OPDecoder op(code);
  if(op.opc == OPCode::calc) {
    if(op.func == ALUCode::ADD) add(op.rd, op.rs1, op.rs2);
    if(op.func == ALUCode::SUB) sub(op.rd, op.rs1, op.rs2);
    if(op.func == ALUCode::AND) land(op.rd, op.rs1, op.rs2);
    if(op.func == ALUCode::OR) lor(op.rd, op.rs1, op.rs2);
    if(op.func == ALUCode::XOR) lxor(op.rd, op.rs1, op.rs2);
    if(op.func == ALUCode::NOT) lnot(op.rd, op.rs1);
    if(op.func == ALUCode::SRS) srs(op.rd, op.rs1);
    if(op.func == ALUCode::SRU) sru(op.rd, op.rs1);
    if(op.func == ALUCode::SL) sl(op.rd, op.rs1);
    if(op.func == ALUCode::EQ) eq(op.rd, op.rs1, op.rs2);
    if(op.func == ALUCode::LTS) lts(op.rd, op.rs1, op.rs2);
    if(op.func == ALUCode::LTU) ltu(op.rd, op.rs1, op.rs2);
    if(op.func == ALUCode::LCAST) lcast(op.rd, op.rs1);
  }
  if(op.opc == OPCode::calci) {
    if(op.func == ALUCode::ADD) addi(op.rd, op.rs1, op.imm);
    if(op.func == ALUCode::SUB) subi(op.rd, op.rs1, op.imm);
    if(op.func == ALUCode::AND) landi(op.rd, op.rs1, op.imm);
    if(op.func == ALUCode::OR) lori(op.rd, op.rs1, op.imm);
    if(op.func == ALUCode::XOR) lxori(op.rd, op.rs1, op.imm);
    if(op.func == ALUCode::EQ) eqi(op.rd, op.rs1, op.rs2);
    if(op.func == ALUCode::LTS) ltsi(op.rd, op.rs1, op.imm);
    if(op.func == ALUCode::LTU) ltui(op.rd, op.rs1, op.imm);
  }
  if(op.opc == OPCode::load) load(op.rd, op.rs1, op.imm);
  if(op.opc == OPCode::store) store(op.rs1, op.rs2, op.imm);
  if(op.opc == OPCode::calif) calif(op.rd, op.rs1, op.rs2, op.imm);
  return;
}

// add rd rs1 rs2
// rd = rs1 + rs2
void CPU::add(uint16_t rd, uint16_t rs1, uint16_t rs2) {
  ram.set(rd, ram.get(rs1) + ram.get(rs2));
  ram.inc_pc();
}

// addi rd rs1 imm
// rd = rs1 + imm
void CPU::addi(uint16_t rd, uint16_t rs1, uint16_t imm) {
  ram.set(rd, ram.get(rs1) + imm);
  ram.inc_pc();
}

// sub rd rs1 rs2
// rd = rs1 - rs2
void CPU::sub(uint16_t rd, uint16_t rs1, uint16_t rs2) {
  ram.set(rd, ram.get(rs1) - ram.get(rs2));
  ram.inc_pc();
}

// subi rd rs1 imm
// rd = rs1 - imm
void CPU::subi(uint16_t rd, uint16_t rs1, uint16_t imm) {
  ram.set(rd, ram.get(rs1) - imm);
  ram.inc_pc();
}

// and rd rs1 rs2
// rd = rs1 & rs2
void CPU::land(uint16_t rd, uint16_t rs1, uint16_t rs2) {
  ram.set(rd, ram.get(rs1) & ram.get(rs2));
  ram.inc_pc();
}

// andi rd rs1 imm
// rd = rs1 & imm
void CPU::landi(uint16_t rd, uint16_t rs1, uint16_t imm) {
  ram.set(rd, ram.get(rs1) & imm);
  ram.inc_pc();
}

// or rd rs1 rs2
// rd = rs1 | rs2
void CPU::lor(uint16_t rd, uint16_t rs1, uint16_t rs2) {
  ram.set(rd, ram.get(rs1) | ram.get(rs2));
  ram.inc_pc();
}

// ori rd rs1 imm
// rd = rs1 | imm
void CPU::lori(uint16_t rd, uint16_t rs1, uint16_t imm) {
  ram.set(rd, ram.get(rs1) | imm);
  ram.inc_pc();
}

// xor rd rs1 rs2
// rd = rs1 ^ rs2
void CPU::lxor(uint16_t rd, uint16_t rs1, uint16_t rs2) {
  ram.set(rd, ram.get(rs1) ^ ram.get(rs2));
  ram.inc_pc();
}

// xori rd rs1 imm
// rd = rs1 ^ imm
void CPU::lxori(uint16_t rd, uint16_t rs1, uint16_t imm) {
  ram.set(rd, ram.get(rs1) ^ imm);
  ram.inc_pc();
}

// not rd rs1
// rd = ~rs1
void CPU::lnot(uint16_t rd, uint16_t rs1) {
  ram.set(rd, ~ram.get(rs1));
  ram.inc_pc();
}

// srs rd rs1
// rd = s>rs1>
void CPU::srs(uint16_t rd, uint16_t rs1) {
  ram.set(rd, ((ram.get(rs1) >> 1) & 0x7fff) | (ram.get(rs1) & 0x8000));
  ram.inc_pc();
}

// sru rd rs1
// rd = 0>rs1>
void CPU::sru(uint16_t rd, uint16_t rs1) {
  ram.set(rd, (ram.get(rs1) >> 1) & 0x7fff);
  ram.inc_pc();
}

// sru rd rs1
// rd = <rs1<
void CPU::sl(uint16_t rd, uint16_t rs1) {
  ram.set(rd, (ram.get(rs1) << 1) & 0xfffe);
  ram.inc_pc();
}

// eq rd rs1 rs2
// rd = (rs1==rs2)
void CPU::eq(uint16_t rd, uint16_t rs1, uint16_t rs2) {
  ram.set(rd, (ram.get(rs1) == ram.get(rs2)) ? 0xffff : 0x0000);
  ram.inc_pc();
}

// eqi rd rs1 rs2
// rd = (rs1==rs2)
void CPU::eqi(uint16_t rd, uint16_t rs1, uint16_t imm) {
  ram.set(rd, (ram.get(rs1) == imm) ? 0xffff : 0x0000);
  ram.inc_pc();
}

// lts rd rs1 rs2
// rd = (rs1 < rs2) signed
void CPU::lts(uint16_t rd, uint16_t rs1, uint16_t rs2) {
  ram.set(rd, ((int16_t)ram.get(rs1) < (int16_t)ram.get(rs2)) ? 0xffff : 0x0000);
  ram.inc_pc();
}

// ltsi rd rs1 imm
// rd = (rs1 < imm) signed
void CPU::ltsi(uint16_t rd, uint16_t rs1, uint16_t imm) {
  ram.set(rd, ((int16_t)ram.get(rs1) < (int16_t)imm) ? 0xffff : 0x0000);
  ram.inc_pc();
}

// ltu rd rs1 rs2
// rd = (rs1 < rs2) unsigned
void CPU::ltu(uint16_t rd, uint16_t rs1, uint16_t rs2) {
  ram.set(rd, (ram.get(rs1) < ram.get(rs2)) ? 0xffff : 0x0000);
  ram.inc_pc();
}

// ltui rd rs1 imm
// rd = (rs1 < imm) unsigned
void CPU::ltui(uint16_t rd, uint16_t rs1, uint16_t imm) {
  ram.set(rd, (ram.get(rs1) < imm) ? 0xffff : 0x0000);
  ram.inc_pc();
}

// lcast rd rs1
// int => bool
void CPU::lcast(uint16_t rd, uint16_t rs1) {
  ram.set(rd, (ram.get(rs1) == 0) ? 0x0000 : 0xffff);
  ram.inc_pc();
}

// load rd rs1 imm
// rd = m[rs1+imm]
void CPU::load(uint16_t rd, uint16_t rs1, uint16_t imm) {
  ram.set(rd, ram.get(ram.get(rs1) + imm));
  ram.inc_pc();
}

// store rs1 rs2 imm
// m[rs1 + imm] = rs2
void CPU::store(uint16_t rs1, uint16_t rs2, uint16_t imm) {
  ram.set(ram.get(rs1) + imm, ram.get(rs2));
  ram.inc_pc();
}

// calif rd rs2 rs1 imm
// rd = pc + 1
// if(rs2==0) pc = rs1 + imm
// else       pc = pc + 1
void CPU::calif(uint16_t rd, uint16_t rs1, uint16_t rs2, uint16_t imm) {
  ram.set(rd, ram.get(Reg::pc) + 1);
  if(ram.get(rs2) == 0)
    ram.set_pc(ram.get(rs1) + imm);
  else
    ram.inc_pc();
}
