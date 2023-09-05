#include "model.hpp"
#include "../rkisa/isa.hpp"
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

void CPU::external_irq(int irq_no) {
  irq_latch[irq_no] = true;
}

void CPU::catch_interrupt() {
  if(ram.get(CSR::irq_en)) {  // 割り込み許可か
    if(irq_latch[0]) ram.set(CSR::irq_0, 1);
    if(irq_latch[1]) ram.set(CSR::irq_1, 1);
    if(irq_latch[2]) ram.set(CSR::irq_2, 1);
    if(irq_latch[3]) ram.set(CSR::irq_3, 1);
  }
}

void CPU::jump_interrupt() {
  if(ram.get(CSR::irq_en)) {  // 割り込み許可か
    if(ram.get(CSR::irq_0)) irq_latch[0] = false, ram.set_ira(), ram.set_pc(Addr::PC_INTR);
    if(ram.get(CSR::irq_1)) irq_latch[0] = false, ram.set_ira(), ram.set_pc(Addr::PC_INTR);
    if(ram.get(CSR::irq_2)) irq_latch[0] = false, ram.set_ira(), ram.set_pc(Addr::PC_INTR);
    if(ram.get(CSR::irq_3)) irq_latch[0] = false, ram.set_ira(), ram.set_pc(Addr::PC_INTR);
  }
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
