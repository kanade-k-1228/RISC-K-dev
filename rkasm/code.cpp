#include "code.hpp"
#include "../rkisa/rkisa.hpp"
#include "../utils/utils.hpp"
#include <iomanip>
#include <iostream>
#include <regex>
#include <sstream>

uint16_t get_reg(std::string);
uint16_t get_op(std::string);
uint16_t get_func(std::string);

void Imm::set(std::string str) {
  try {
    this->value = std::stoi(str, nullptr, 0);
    this->type = LITERAL;
  } catch(std::out_of_range& e) {
    error("Immidiate Out of Range: " + str);
  } catch(std::invalid_argument& e) {
    this->label = str;
    this->type = LAB_REF;
  }
  return;
}

void Operation::set(const uint16_t address, const std::vector<std::string> code_s) {
  addr = address;
  str = code_s;
  op = get_op(str.at(0));
  if(op == CALC) {
    func = get_func(str.at(0));
    rd = get_reg(str.at(1));
    rs1 = get_reg(str.at(2));
    rs2 = get_reg(str.at(3));
  }
  if(op == CALCI) {
    func = get_func(str.at(0));
    rd = get_reg(str.at(1));
    rs1 = get_reg(str.at(2));
    imm.set(str.at(3));
  }
  if(op == LOAD) {
    rd = get_reg(str.at(1));
    rs1 = get_reg(str.at(2));
    imm.set(str.at(3));
  }
  if(op == LOADI) {
    rd = get_reg(str.at(1));
    imm.set(str.at(2));
  }
  if(op == STORE) {
    rs2 = get_reg(str.at(1));
    rs1 = get_reg(str.at(2));
    imm.set(str.at(3));
  }
  if(op == JUMP) {
    rd = get_reg(str.at(1));
    rs1 = get_reg(str.at(2));
    imm.set(str.at(3));
  }
  if(op == BREQ || op == BRLT) {
    rs1 = get_reg(str.at(1));
    rs2 = get_reg(str.at(2));
    imm.set(str.at(3));
  }
}

uint16_t get_reg(std::string name) {
  if(name == "zero") return ZERO;
  if(name == "ra") return RA;
  if(name == "sp") return SP;
  if(name == "fp") return FP;
  if(name == "csr") return CSR;
  if(name == "ira") return IRA;
  if(name == "cout") return COUT;
  if(name == "os0") return OS0;
  if(name == "os1") return OS1;
  if(name == "os2") return OS2;
  if(name == "os3") return OS3;
  if(name == "s0") return S0;
  if(name == "s1") return S1;
  if(name == "s2") return S2;
  if(name == "s3") return S3;
  if(name == "t0") return T0;
  if(name == "t1") return T1;
  if(name == "t2") return T2;
  if(name == "t3") return T3;
  if(name == "a0") return A0;
  if(name == "a1") return A1;
  if(name == "a2") return A2;
  if(name == "a3") return A3;
  error("Invalid Registor Name: " + name);
  return 0;
}

uint16_t get_op(std::string op_s) {
  if(op_s == "add" || op_s == "sub" || op_s == "and" || op_s == "or" || op_s == "xor" || op_s == "not" || op_s == "lrot" || op_s == "rrot") return CALC;
  if(op_s == "addi" || op_s == "subi" || op_s == "andi" || op_s == "ori" || op_s == "xori") return CALCI;
  if(op_s == "load") return LOAD;
  if(op_s == "store") return STORE;
  if(op_s == "loadi") return LOADI;
  if(op_s == "jump") return JUMP;
  if(op_s == "breq") return BREQ;
  if(op_s == "brlt") return BRLT;
  error("Invalid Operation Name: " + op_s);
  return 0;
}

uint16_t get_func(std::string op_s) {
  if(op_s == "add" || op_s == "addi") return ADD;
  if(op_s == "sub" || op_s == "subi") return SUB;
  if(op_s == "and" || op_s == "andi") return AND;
  if(op_s == "or" || op_s == "ori") return OR;
  if(op_s == "xor" || op_s == "xori") return XOR;
  if(op_s == "not") return NOT;
  if(op_s == "lrot") return LROT;
  if(op_s == "rrot") return RROT;
  error("Invalid Operation Name: " + op_s);
  return 0;
}

uint32_t Operation::get_bin() {
  bin = 0;
  if(op == CALC) {
    bin |= (rs1 & 0x3F);
    bin |= (op & 0x0F) << 6;
    bin |= (func & 0x0F) << 10;
    bin |= (rs2 & 0x3F) << 20;
    bin |= (rd & 0x3F) << 26;
  } else if(op == CALCI) {
    bin |= (rs1 & 0x3F);
    bin |= (op & 0x0F) << 6;
    bin |= (imm.value & 0x3FF) << 10;
    bin |= (func & 0x0F) << 20;
    bin |= (imm.value & 0xA00) >> 10 << 24;
    bin |= (rd & 0x3F) << 26;
  } else if(op == LOAD || op == LOADI || op == JUMP) {
    bin |= (rs1 & 0x3F);
    bin |= (op & 0x0F) << 6;
    bin |= (imm.value & 0xFFFF) << 10;
    bin |= (rd & 0x3F) << 26;
  } else if(op == STORE || op == BREQ || op == BRLT) {
    bin |= (rs1 & 0x3F);
    bin |= (op & 0x0F) << 6;
    bin |= (imm.value & 0x3FF) << 10;
    bin |= (rs2 & 0x3F) << 20;
    bin |= (imm.value & 0xFA00) >> 10 << 26;
  } else {
    error("Invalid Code Format");
  }
  return bin;
}

void Label::set(const uint16_t address, const std::vector<std::string> str) {
  std::string str0 = str.at(0);
  if(str0.at(str0.size() - 1) == ':') {
    type = OPR;
    str0.pop_back();
    name = str0;
    value = address;
  } else if(str0.at(0) == '@') {
    type = VAR;
    name = str.at(1);
    str0.erase(str0.begin());
    value = std::stoi(str0, nullptr, 0);
  } else if(str0.at(0) == '#') {
    type = CONST;
    name = str.at(1);
    str0.erase(str0.begin());
    value = std::stoi(str0, nullptr, 0);
  }
}

Code::Code(const uint16_t address, const std::vector<std::string> code_s)
    : code_s(code_s) {
  if(std::regex_search(code_s.at(0), std::regex("^("
                                                "add|sub|and|or|xor"
                                                "|not|lrot|rrot"
                                                "|addi|subi|andi|ori|xori"
                                                "|store|load|loadi|jump|breq|brlt"
                                                ")$"))) {
    // 命令行の場合
    type = OPERATION;
    opr.set(address, code_s);
  } else if(std::regex_search(code_s.at(0), std::regex("(:$)|(^@)|(^#)"))) {
    // ラベル定義の場合
    type = LABEL_DEF;
    label.set(address, code_s);
  }
}
