#include "code.hpp"
#include "../rkisa/rkisa.hpp"
#include "../utils/utils.hpp"
#include <iomanip>
#include <iostream>
#include <regex>
#include <sstream>

bool is_op(std::string);
bool is_opr_lab(std::string);
bool is_var_lab(std::string);

uint16_t get_reg(std::string);
uint16_t get_imm(std::string);
uint16_t get_op(std::string);
uint16_t get_func(std::string);

Code::Code(const uint16_t address, const std::vector<std::string> code_s)
    : addr(address), code_s(code_s) {
  // ラベル行の場合
  if(is_op(code_s.at(0))) {  // 命令行
    op_s = code_s.at(0);
    op = get_op(op_s);
    if(op == CALC) {
      func = get_func(op_s);
      rd = get_reg(code_s.at(1));
      rs1 = get_reg(code_s.at(2));
      rs2 = get_reg(code_s.at(3));
    }
    if(op == CALCI) {
      func = get_func(op_s);
      rd = get_reg(code_s.at(1));
      rs1 = get_reg(code_s.at(2));
      imm = get_imm(code_s.at(3));
    }
    if(op == LOAD) {
      rd = get_reg(code_s.at(1));
      rs1 = get_reg(code_s.at(2));
      set_imm_or_label(code_s.at(3));
    }
    if(op == LOADI) {
      rd = get_reg(code_s.at(1));
      set_imm_or_label(code_s.at(2));
    }
    if(op == STORE) {
      rs2 = get_reg(code_s.at(1));
      rs1 = get_reg(code_s.at(2));
      set_imm_or_label(code_s.at(3));
    }
    if(op == JUMP) {
      rd = get_reg(code_s.at(1));
      rs1 = get_reg(code_s.at(2));
      set_imm_or_label(code_s.at(3));
      return;
    }
    if(op == BREQ || op == BRLT) {
      rs1 = get_reg(code_s.at(1));
      rs2 = get_reg(code_s.at(2));
      set_imm_or_label(code_s.at(3));
      return;
    }
  } else if(is_opr_lab(code_s.at(0))) {  // ラベル行
    opr_lab_def = true;
    lab_str = code_s.at(0);
    lab_str.pop_back();
    return;
  } else if(is_var_lab(code_s.at(0))) {  // ラベル行
    var_lab_def = true;
    std::string addr = code_s.at(0);
    addr.erase(addr.begin());
    imm = std::stoi(addr, nullptr, 0);
    lab_str = code_s.at(1);
    return;
  }
}

bool is_op(std::string str) {
  std::regex op("^("
                "add|sub|and|or|xor"
                "|not|lrot|rrot"
                "|addi|subi|andi|ori|xori"
                "|store|load|loadi|jump|breq|brlt"
                ")$");
  return std::regex_search(str, op);
}

bool is_opr_lab(std::string str) {
  return std::regex_search(str, std::regex(":$"));
}

bool is_var_lab(std::string str) {
  return std::regex_search(str, std::regex("^@"));
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

uint16_t get_imm(std::string imm) {
  try {
    return std::stoi(imm, nullptr, 0);
  } catch(std::invalid_argument& e) {
    error("Expect Number");
    return 0;
  } catch(std::out_of_range& e) {
    error("Immidiate Out of Range");
    return 0;
  }
}

void Code::set_imm_or_label(const std::string str) {
  try {
    this->imm = std::stoi(str, nullptr, 0);
  } catch(std::out_of_range& e) {
    error("Immidiate Out of Range: " + str);
  } catch(std::invalid_argument& e) {
    lab_str = str;
    lab_ref = true;
  }
  return;
}

uint32_t Code::get_bin() {
  code = 0;
  if(op == CALC) {
    code |= (rs1 & 0x3F);
    code |= (op & 0x0F) << 6;
    code |= (func & 0x0F) << 10;
    code |= (rs2 & 0x3F) << 20;
    code |= (rd & 0x3F) << 26;
  } else if(op == CALCI) {
    code |= (rs1 & 0x3F);
    code |= (op & 0x0F) << 6;
    code |= (imm & 0x3FF) << 10;
    code |= (func & 0x0F) << 20;
    code |= (imm & 0xA00) >> 10 << 24;
    code |= (rd & 0x3F) << 26;
  } else if(op == LOAD || op == LOADI || op == JUMP) {
    code |= (rs1 & 0x3F);
    code |= (op & 0x0F) << 6;
    code |= (imm & 0xFFFF) << 10;
    code |= (rd & 0x3F) << 26;
  } else if(op == STORE || op == BREQ || op == BRLT) {
    code |= (rs1 & 0x3F);
    code |= (op & 0x0F) << 6;
    code |= (imm & 0x3FF) << 10;
    code |= (rs2 & 0x3F) << 20;
    code |= (imm & 0xFA00) >> 10 << 26;
  } else {
    error("Invalid Code Format");
  }
  return code;
}
