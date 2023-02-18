#include "code.hpp"
#include "../utils/utils.hpp"
#include <sstream>

void Code::comment(std::string comment) { code.push_back("; " + comment); }

void Code::label(std::string label) { code.push_back(label + ":"); };
void Code::addr_label(std::string label, uint16_t addr) { code.push_back("@" + hex(true, addr) + " " + label); };
void Code::const_label(std::string label, uint16_t value) { code.push_back("#" + hex(true, value) + " " + label); };

void Code::add(std::string rd, std::string rs1, std::string rs2) { code.push_back("  add   " + rd + " " + rs1 + " " + rs2); }
void Code::sub(std::string rd, std::string rs1, std::string rs2) { code.push_back("  sub   " + rd + " " + rs1 + " " + rs2); }
void Code::addi(std::string rd, std::string rs, int imm) { code.push_back("  addi  " + rd + " " + rs + " " + std::to_string(imm)); }
void Code::subi(std::string rd, std::string rs, int imm) { code.push_back("  subi  " + rd + " " + rs + " " + std::to_string(imm)); }

void Code::load(std::string rd, std::string addr_base, int addr_imm) { code.push_back("  load  " + rd + " " + addr_base + " " + std::to_string(addr_imm)); }
void Code::load(std::string rd, std::string addr_base, std::string label) { code.push_back("  load  " + rd + " " + addr_base + " " + label); }
void Code::loadi(std::string rd, int imm) { code.push_back("  loadi " + rd + " " + std::to_string(imm)); }
void Code::loadi(std::string rd, std::string label) { code.push_back("  loadi " + rd + " " + label); }

void Code::store(std::string src, std::string addr_base, int addr_imm) { code.push_back("  store " + addr_base + " " + src + " " + std::to_string(addr_imm)); }
void Code::store(std::string src, std::string addr_base, std::string label) { code.push_back("  store " + addr_base + " " + src + " " + label); }

void Code::jump(std::string rd, std::string rs, int addr) { code.push_back("  jump  " + rd + " " + rs + " " + std::to_string(addr)); }
void Code::jump(std::string rd, std::string rs, std::string label) { code.push_back("  jump  " + rd + " " + rs + " " + label); }

void Code::breq(std::string rs1, std::string rs2, int addr) { code.push_back("  breq  " + rs1 + " " + rs2 + " " + std::to_string(addr)); }
void Code::breq(std::string rs1, std::string rs2, std::string label) { code.push_back("  breq  " + rs1 + " " + rs2 + " " + label); }

// 拡張命令
void Code::push(std::string reg) {
  store(reg, "sp", 0);
  subi("sp", "sp", 1);
}
void Code::pop(std::string reg) {
  addi("sp", "sp", 1);
  load(reg, "sp", 0);
}

std::ostream& operator<<(std::ostream& ss, Code asmcode) {
  for(auto line : asmcode.code) ss << line << std::endl;
  return ss;
}
