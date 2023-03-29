#include "code.hpp"
#include "../rkisa/rkisa.hpp"
#include "../utils/utils.hpp"
#include "reader.hpp"
#include <iomanip>
#include <iostream>
#include <regex>
#include <sstream>

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

std::string Imm::print() {
  if(type == Imm::LITERAL) return cprint(hex(true, value), YELLOW, 8);
  if(type == Imm::OPR_LAB_REF) return cprint(hex(true, value), GREEN, 8) + cprint(" = " + label, GREEN, 0);
  if(type == Imm::VAR_LAB_REF) return cprint(hex(true, value), BLUE, 8) + cprint(" = " + label, BLUE, 0);
  if(type == Imm::CONST_LAB_REF) return cprint(hex(true, value), YELLOW, 8) + cprint(" = " + label, YELLOW, 0);
  return "";
}
void Operation::set(const uint16_t address, const std::vector<std::string> code_s) {
  addr = address;
  str = code_s;
  std::string s = str.at(0);
  if(is_calc(s)) {
    opc = OP::CALC;
    func = str_to_func(s);
    rd = str_to_reg(str.at(1));
    rs1 = str_to_reg(str.at(2));
    rs2 = str_to_reg(str.at(3));
  }
  if(is_calci(s)) {
    opc = OP::CALCI;
    func = str_to_func(s);
    rd = str_to_reg(str.at(1));
    rs1 = str_to_reg(str.at(2));
    imm.set(str.at(3));
  }
  if(s == "load") {
    opc = OP::LOAD;
    rd = str_to_reg(str.at(1));
    rs1 = str_to_reg(str.at(2));
    imm.set(str.at(3));
  }
  if(s == "store") {
    opc = OP::STORE;
    rs2 = str_to_reg(str.at(1));
    rs1 = str_to_reg(str.at(2));
    imm.set(str.at(3));
  }
  if(s == "callif") {
    opc == OP::JMPIF;
    rd = str_to_reg(str.at(1));
    rs2 = str_to_reg(str.at(2));
    rs1 = str_to_reg(str.at(3));
    imm.set(str.at(4));
  }
  // Pseudo Operation
  if(s == "loadi") {
    opc = OP::CALCI;
    func = Func::ADD;
    rd = str_to_reg(str.at(1));
    rs1 = str_to_reg("zero");
    imm.set(str.at(2));
  }
  if(s == "if") {
    opc = OP::JMPIF;
    rd = str_to_reg("zero");
    rs2 = str_to_reg(str.at(1));
    rs1 = str_to_reg("zero");
    imm.set(str.at(2));
  }
  if(s == "jmp") {
    opc = OP::JMPIF;
    rd = str_to_reg("zero");
    rs2 = str_to_reg("zero");
    rs1 = str_to_reg("zero");
    imm.set(str.at(1));
  }
  if(s == "call") {
    opc = OP::JMPIF;
    rd = str_to_reg("ra");
    rs2 = str_to_reg("zero");
    rs1 = str_to_reg("zero");
    imm.set(str.at(1));
  }
  if(s == "ret") {
    opc = OP::JMPIF;
    rd = str_to_reg("zero");
    rs2 = str_to_reg("zero");
    rs1 = str_to_reg("ra");
    imm.set("0x00");
  }
  if(s == "iret") {
    opc = OP::JMPIF;
    rd = str_to_reg("zero");
    rs2 = str_to_reg("zero");
    rs1 = str_to_reg("ira");
    imm.set("0x00");
  }
}

uint32_t Operation::get_bin() {
  bin = OPEncoder(opc, func, rs1, rs2, rd, imm.value).bin;
  return bin;
}

std::string Operation::print() {
  std::string s = str.at(0);
  std::stringstream ss;
  ss << cprint(str.at(0), RED, 6);
  if(is_calc(s)) ss << cprint(str.at(1), BLUE, 6) << cprint(str.at(2), BLUE, 8) << cprint(str.at(3), BLUE, 8);
  if(is_calci(s)) ss << cprint(str.at(1), BLUE, 6) << cprint(str.at(2), BLUE, 8) << imm.print();
  if(s == "load") ss << cprint(str.at(1), BLUE, 6) << cprint(str.at(2), BLUE, 8) << imm.print();
  if(s == "store") ss << cprint(str.at(1), BLUE, 6) << cprint(str.at(2), BLUE, 8) << imm.print();
  if(s == "callif") ss << cprint(str.at(1), BLUE, 6) << cprint(str.at(2), BLUE, 8) << cprint(str.at(3), BLUE, 8) << imm.print();
  // Pseudo Operation
  if(s == "loadi" || s == "if") ss << cprint(str.at(1), BLUE, 6) << "        " << imm.print();
  if(s == "jmp" || s == "call") ss << "              " << imm.print();
  // if(s == "ret" || s == "iret")
  return ss.str();
}


void LabelDef::set(const uint16_t address, const std::vector<std::string> str) {
  std::string str0 = str.at(0);
  if(str0.at(str0.size() - 1) == ':') {  // 命令ラベル hoge:
    type = OPR;
    str0.pop_back();
    name = str0;
    value = address;
  } else if(str0.at(0) == '@') {  // 変数ラベル @0x00 hoge
    type = VAR;
    name = str.at(1);
    str0.erase(str0.begin());
    value = std::stoi(str0, nullptr, 0);
  } else if(str0.at(0) == '#') {  // 定数ラベル #0x00 hoge
    type = CONST;
    name = str.at(1);
    str0.erase(str0.begin());
    value = std::stoi(str0, nullptr, 0);
  }
}

ASMLine::ASMLine(const uint16_t address, const std::vector<std::string> code_s)
    : code_s(code_s) {
  if(std::regex_search(code_s.at(0), std::regex("^(" + mnemonic + "|" + pseudo_mnemonic + ")$"))) {
    // 命令行の場合
    type = OPERATION;
    opr.set(address, code_s);
  } else if(std::regex_search(code_s.at(0), std::regex("(:$)|(^@)|(^#)"))) {
    // ラベル定義の場合
    type = LABEL_DEF;
    label.set(address, code_s);
  } else {
    error("Unknown Code: " + code_s.at(0));
  }
}
