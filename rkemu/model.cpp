#include "model.hpp"
#include "../rkarch/arch.hpp"
#include "utils.hpp"
#include <bitset>
#include <iomanip>
#include <sstream>

uint16_t decode_imm(uint32_t, uint16_t);
uint16_t decode_func(uint32_t, uint16_t);

CPU::CPU() : rom{0} {}

void CPU::load_rom_file(std::string fname) {
  this->fname = fname;
  std::ifstream f(fname);
  if(!f) throw new std::string("Cant Open File: " + fname);
  uint32_t buf;
  for(int op_cnt = 0; !f.eof(); op_cnt++) {
    f.read((char*)&buf, sizeof(uint32_t));
    rom.at(op_cnt) = buf;
  }
}

int CPU::serial() {
  char cout = ram.get(Serial::tx);
  if(cout) {
    ram.set(Serial::tx, 0);
    return cout;
  } else {
    return -1;
  }
}

void CPU::interrupt(uint16_t irq_flag) {
  static int irq_flag_latch;
  static const uint16_t irq_0 = 1;
  static const uint16_t irq_1 = 2;
  static const uint16_t irq_2 = 4;
  static const uint16_t irq_3 = 8;
  if(ram.get(CSR::irq_en)) {
    if(irq_flag_latch & (irq_0 | irq_1 | irq_2 | irq_3)) {
      ram.set_ira();
      ram.set_pc(Addr::PC_INTR);
    }
  }
  irq_flag_latch = irq_flag;
}

void CPU::execute(uint32_t code) {
  Decoder op(code);
  if(op.opc == OPCode::calc) calc(op.func, op.rd, op.rs1, op.rs2);
  if(op.opc == OPCode::calci) calci(op.func, op.rd, op.rs1, op.imm);
  if(op.opc == OPCode::load) load(op.rd, op.rs1, op.imm);
  if(op.opc == OPCode::store) store(op.rs1, op.rs2, op.imm);
  if(op.opc == OPCode::ctrl) ctrl(op.rd, op.rs1, op.rs2, op.imm);
  return;
}

// calc rd rs1 rs2
// rd = rs1 + rs2
void CPU::calc(uint8_t alu_func, uint16_t rd, uint16_t rs1, uint16_t rs2) {
  ram.set(rd, alu(alu_func, ram.get(rs1), ram.get(rs2)));
  ram.inc_pc();
}

// calci rd rs1 imm
// rd = rs1 + imm
void CPU::calci(uint8_t alu_func, uint16_t rd, uint16_t rs1, uint16_t imm) {
  ram.set(rd, alu(alu_func, ram.get(rs1), imm));
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

// ctrl rd rs2 rs1 imm
// rd = pc + 1
// if(rs2==0) pc = rs1 + imm
// else       pc = pc + 1
void CPU::ctrl(uint16_t rd, uint16_t rs1, uint16_t rs2, uint16_t imm) {
  ram.set(rd, ram.get(Reg::pc) + 1);
  if(ram.get(rs2) == 0)
    ram.set_pc(ram.get(rs1) + imm);
  else
    ram.inc_pc();
}
