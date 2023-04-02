#include "operation.hpp"
#include "../rkisa/rkisa.hpp"
#include "utils.hpp"
#include <sstream>

bool is_calc(std::string s) {
  return s == "add" || s == "sub"
         || s == "and" || s == "or" || s == "xor" || s == "not"
         || s == "srs" || s == "sru" || s == "sl"
         || s == "lnot" || s == "eq" || s == "lts" || s == "ltu";
}

bool is_calci(std::string s) {
  return s == "addi" || s == "subi"
         || s == "andi" || s == "ori" || s == "xori"
         || s == "eqi" || s == "ltsi" || s == "ltui";
}

uint16_t reg_stoi(std::string name) {
  if(name == "zero") return ZERO;
  if(name == "pc") return PC;
  if(name == "ira") return IRA;
  if(name == "csr") return CSR;
  if(name == "ra") return RA;
  if(name == "sp") return SP;
  if(name == "fp") return FP;
  if(name == "cout") return COUT;
  if(name == "s0") return S0;
  if(name == "s1") return S1;
  if(name == "s2") return S2;
  if(name == "s3") return S3;
  if(name == "t0") return T0;
  if(name == "t1") return T1;
  if(name == "t2") return T2;
  if(name == "t3") return T3;
  error("Invalid Registor Name: " + name);
  return 0;
}

uint16_t str_to_func(std::string s) {
  if(s == "add" || s == "addi") return ADD;
  if(s == "sub" || s == "subi") return SUB;
  if(s == "and" || s == "andi") return AND;
  if(s == "or" || s == "ori") return OR;
  if(s == "xor" || s == "xori") return XOR;
  if(s == "not") return NOT;
  if(s == "srs") return SRS;
  if(s == "sru") return SRU;
  if(s == "sl") return SL;
  if(s == "eq" || s == "eqi") return EQ;
  if(s == "lts" || s == "ltsi") return LTS;
  if(s == "ltu" || s == "ltui") return LTU;
  if(s == "lnot") return LNOT;
  error("Invalid Operation Name: " + s);
  return 0;
}

Operation::Operation(
    const uint16_t address,
    const std::vector<std::string> str)
    : address(address), mnemonic(str.at(0)), rd("zero"), rs1("zero"), rs2("zero") {
  if(is_calc(mnemonic)) {
    rd = str.at(1);
    rs1 = str.at(2);
    rs2 = str.at(3);
  }
  if(is_calci(mnemonic)) {
    rd = str.at(1);
    rs1 = str.at(2);
    imm = Imm(str.at(3));
  }
  if(mnemonic == "load") {
    rd = str.at(1);
    rs1 = str.at(2);
    imm = Imm(str.at(3));
  }
  if(mnemonic == "store") {
    rs2 = str.at(1);
    rs1 = str.at(2);
    imm = Imm(str.at(3));
  }
  if(mnemonic == "jmpif") {
    rs2 = str.at(1);
    rs1 = str.at(2);
    imm = Imm(str.at(3));
  }
  if(mnemonic == "fctrl") {
    rd = str.at(1);
    rs2 = str.at(2);
    imm = Imm(str.at(3));
  }
  // Pseudo Operation
  if(mnemonic == "mov") {
    rd = str.at(1);
    rs1 = str.at(2);
    imm = Imm("zero");
  }
  if(mnemonic == "loadi") {
    rd = str.at(1);
    rs1 = "zero";
    imm = Imm(str.at(2));
  }
  if(mnemonic == "call") {
    rd = "ra";
    rs2 = "zero";
    rs1 = "zero";
    imm = Imm(str.at(1));
  }
  if(mnemonic == "ret") {
    rd = "zero";
    rs2 = "zero";
    rs1 = "ra";
    imm = Imm("0x00");
  }
  if(mnemonic == "iret") {
    rd = "zero";
    rs2 = "zero";
    rs1 = "ira";
    imm = Imm("0x00");
  }
}

uint32_t pack(uint8_t u4_0, uint8_t u4_1, uint8_t u4_2, uint8_t u4_3, uint16_t u16) {
  return (u4_0 & 0x0F)
         | ((u4_1 & 0x0F) << 4)
         | ((u4_2 & 0x0F) << 8)
         | ((u4_3 & 0x0F) << 12)
         | ((u16 & 0xFFFF) << 16);
}

uint32_t Operation::get_bin() {
  uint32_t ret;
  if(is_calc(mnemonic)) return pack(reg_stoi(rs1), reg_stoi(rs2), reg_stoi(rd), OPCode::calc, str_to_func(mnemonic));
  if(is_calci(mnemonic)) return pack(reg_stoi(rs1), str_to_func(mnemonic), reg_stoi(rd), OPCode::calci, imm.value);
  if(mnemonic == "load") return pack(reg_stoi(rs1), 0, reg_stoi(rd), OPCode::load, imm.value);
  if(mnemonic == "store") return pack(reg_stoi(rs1), reg_stoi(rs2), 0, OPCode::store, imm.value);
  if(mnemonic == "jmpif") return pack(reg_stoi(rs1), reg_stoi(rs2), 0, OPCode::jmpif, imm.value);
  if(mnemonic == "fctrl") return pack(reg_stoi(rs1), 0, reg_stoi(rd), OPCode::fctrl, imm.value);
  // Pseudo Operation
  if(mnemonic == "mov") return pack(reg_stoi(rs1), reg_stoi(rs2), 0, OPCode::calc, Func::ADD);
  if(mnemonic == "loadi") return pack(0, Func::ADD, reg_stoi(rd), OPCode::calci, imm.value);
  if(mnemonic == "call" || mnemonic == "ret" || mnemonic == "iret")
    return pack(reg_stoi(rs1), 0, reg_stoi(rd), OPCode::fctrl, imm.value);
  return ret;
}

std::string Operation::print() {
  std::stringstream ss;
  ss << cprint(mnemonic, RED, 6);
  if(is_calc(mnemonic))
    ss << cprint(rd, BLUE, 6) << cprint(rs1, BLUE, 8) << cprint(rs2, BLUE, 8);
  else if(is_calci(mnemonic) || mnemonic == "load" || mnemonic == "fctrl")
    ss << cprint(rd, BLUE, 6) << cprint(rs1, BLUE, 8) << imm.print();
  else if(mnemonic == "store" || mnemonic == "jmpif")
    ss << cprint(rs2, BLUE, 6) << cprint(rs1, BLUE, 8) << imm.print();
  // Pseudo Operation
  else if(mnemonic == "mov")
    ss << cprint(rd, BLUE, 6) << "        " << cprint(rs1, BLUE, 8);
  else if(mnemonic == "loadi" || mnemonic == "if")
    ss << cprint(rd, BLUE, 6) << "        " << imm.print();
  else if(mnemonic == "jmp" || mnemonic == "call")
    ss << "              " << imm.print();
  return ss.str();
}
