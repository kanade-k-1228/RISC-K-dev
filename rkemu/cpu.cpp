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
  std::ifstream f(fname);
  if(!f) error("Cant Open File: " + fname);
  uint32_t buf;
  for(int op_cnt = 0; !f.eof(); op_cnt++) {
    f.read((char*)&buf, sizeof(uint32_t));
    rom.at(op_cnt) = buf;
  }
}

void CPU::dump() {
  std::cout << "------------------------------" << std::endl
            << "         | " << cprint("Save", BLUE, 0) << " | " << cprint("Temp", BLUE, 0) << " | " << cprint("Argu", BLUE, 0) << std::endl
            << cprint("PC: " + hex(false, pc), GREEN, 0) << " | " << hex(false, mem.at(S0)) << " | " << hex(false, mem.at(T0)) << " | " << hex(false, mem.at(A0)) << std::endl
            << "RA: " << hex(false, mem.at(RA)) << " | " << hex(false, mem.at(S1)) << " | " << hex(false, mem.at(T1)) << " | " << hex(false, mem.at(A1)) << std::endl
            << "SP: " << hex(false, mem.at(SP)) << " | " << hex(false, mem.at(S2)) << " | " << hex(false, mem.at(T2)) << " | " << hex(false, mem.at(A2)) << std::endl
            << "FP: " << hex(false, mem.at(FP)) << " | " << hex(false, mem.at(S3)) << " | " << hex(false, mem.at(T3)) << " | " << hex(false, mem.at(A3)) << std::endl
            << "CSR: " << std::bitset<16>(mem.at(CSR)) << std::endl
            << "------------------------------" << std::endl;
  for(uint16_t sp = mem.at(SP); sp < mem.at(FP); sp++)
    std::cout << hex(false, (uint16_t)(sp + 1)) << " : " << hex(false, mem.at(sp + 1)) << std::endl;
  std::cout << "------------------------------" << std::endl;
}

void CPU::cstop() {
  if(mem.at(CSR) & CSRBit::CSTOP) {
    std::cout << std::endl
              << "-----------------------------#" << std::endl;
    exit(EXIT_SUCCESS);
  }
}

void CPU::serial(bool single_line) {
  if((mem.at(CSR) & SSEND) == SSEND) {
    std::cout << (single_line ? "> '" : "")
              << (char)mem.at(COUT)
              << (single_line ? "'\n" : "");
    mem.at(CSR) &= ~SSEND;
  }
}

void CPU::interrupt(int intr_no) {
  if(intr_no == 0) mem.at(CSR) |= INTR0;
  if(intr_no == 1) mem.at(CSR) |= INTR1;
  if(intr_no == 2) mem.at(CSR) |= INTR2;
  if(intr_no == 3) mem.at(CSR) |= INTR3;
  pc = PC_INTR;
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

void CPU::print_code(uint32_t code) {
  uint16_t opc = (code >> 6) & 0x000f;
  uint16_t func = decode_func(code, opc);
  uint16_t rs1 = (code >> 0) & 0x003f;
  uint16_t rs2 = (code >> 20) & 0x003f;
  uint16_t rd = (code >> 26) & 0x003f;
  uint16_t imm = decode_imm(code, opc);

  if(opc == CALC) {
    if(func == ADD) std::cout << cprint("add", RED, 6) << cprint(hex(false, rd), BLUE, 6) << cprint(hex(false, rs1), BLUE, 6) << cprint(hex(false, rs2), BLUE, 6) << std::endl;
    if(func == SUB) std::cout << cprint("sub", RED, 6) << cprint(hex(false, rd), BLUE, 6) << cprint(hex(false, rs1), BLUE, 6) << cprint(hex(false, rs2), BLUE, 6) << std::endl;
    if(func == AND) std::cout << cprint("and", RED, 6) << cprint(hex(false, rd), BLUE, 6) << cprint(hex(false, rs1), BLUE, 6) << cprint(hex(false, rs2), BLUE, 6) << std::endl;
    if(func == OR) std::cout << cprint("or", RED, 6) << cprint(hex(false, rd), BLUE, 6) << cprint(hex(false, rs1), BLUE, 6) << cprint(hex(false, rs2), BLUE, 6) << std::endl;
    if(func == XOR) std::cout << cprint("xor", RED, 6) << cprint(hex(false, rd), BLUE, 6) << cprint(hex(false, rs1), BLUE, 6) << cprint(hex(false, rs2), BLUE, 6) << std::endl;
    if(func == NOT) std::cout << cprint("not", RED, 6) << cprint(hex(false, rd), BLUE, 6) << cprint(hex(false, rs1), BLUE, 6) << std::endl;
    if(func == LROT) std::cout << cprint("lrot", RED, 6) << cprint(hex(false, rd), BLUE, 6) << cprint(hex(false, rs1), BLUE, 6) << std::endl;
    if(func == RROT) std::cout << cprint("rrot", RED, 6) << cprint(hex(false, rd), BLUE, 6) << cprint(hex(false, rs1), BLUE, 6) << std::endl;
  }
  if(opc == CALCI) {
    if(func == ADD) std::cout << cprint("addi", RED, 6) << cprint(hex(false, rd), BLUE, 6) << cprint(hex(false, rs1), BLUE, 6) << cprint(hex(false, imm), YELLOW, 6) << std::endl;
    if(func == SUB) std::cout << cprint("subi", RED, 6) << cprint(hex(false, rd), BLUE, 6) << cprint(hex(false, rs1), BLUE, 6) << cprint(hex(false, imm), YELLOW, 6) << std::endl;
    if(func == AND) std::cout << cprint("andi", RED, 6) << cprint(hex(false, rd), BLUE, 6) << cprint(hex(false, rs1), BLUE, 6) << cprint(hex(false, imm), YELLOW, 6) << std::endl;
    if(func == OR) std::cout << cprint("ori", RED, 6) << cprint(hex(false, rd), BLUE, 6) << cprint(hex(false, rs1), BLUE, 6) << cprint(hex(false, imm), YELLOW, 6) << std::endl;
    if(func == XOR) std::cout << cprint("xori", RED, 6) << cprint(hex(false, rd), BLUE, 6) << cprint(hex(false, rs1), BLUE, 6) << cprint(hex(false, imm), YELLOW, 6) << std::endl;
  }
  if(opc == LOAD) std::cout << cprint("load", RED, 6) << cprint(hex(false, rd), BLUE, 6) << cprint(hex(false, rs1), BLUE, 6) << cprint(hex(false, imm), YELLOW, 6) << std::endl;
  if(opc == LOADI) std::cout << cprint("loadi", RED, 6) << cprint(hex(false, rd), BLUE, 6) << cprint(hex(false, imm), YELLOW, 6) << std::endl;
  if(opc == STORE) std::cout << cprint("store", RED, 6) << cprint(hex(false, rs2), BLUE, 6) << cprint(hex(false, rs1), BLUE, 6) << cprint(hex(false, imm), YELLOW, 6) << std::endl;
  if(opc == JUMP) std::cout << cprint("jump", RED, 6) << cprint(hex(false, rd), BLUE, 6) << cprint(hex(false, rs1), BLUE, 6) << cprint(hex(false, imm), YELLOW, 6) << std::endl;
  if(opc == BREQ) std::cout << cprint("breq", RED, 6) << cprint(hex(false, rs1), BLUE, 6) << cprint(hex(false, rs2), BLUE, 6) << cprint(hex(false, imm), YELLOW, 6) << std::endl;
  if(opc == BRLT) std::cout << cprint("brlt", RED, 6) << cprint(hex(false, rs1), BLUE, 6) << cprint(hex(false, rs2), BLUE, 6) << cprint(hex(false, imm), YELLOW, 6) << std::endl;
  return;
}
