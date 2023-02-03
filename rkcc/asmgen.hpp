#include "ast.hpp"

namespace Asm {
extern std::vector<std::string> code;
void comment(std::string comment);
void label(std::string label);
void add(std::string rd, std::string rs1, std::string rs2);
void sub(std::string rd, std::string rs1, std::string rs2);
void addi(std::string rd, std::string rs, int imm);
void subi(std::string rd, std::string rs, int imm);
void load(std::string rd, std::string addr_base, int addr_imm);
void load(std::string rd, std::string addr_base, std::string label);
void loadi(std::string rd, int imm);
void loadi(std::string rd, std::string label);
void store(std::string src, std::string addr_base, int addr_imm);
void store(std::string src, std::string addr_base, std::string label);
void jump(std::string rd, std::string rs, int addr);
void jump(std::string rd, std::string rs, std::string label);
void breq(std::string rs1, std::string rs2, int addr);
void breq(std::string rs1, std::string rs2, std::string label);
void push(std::string reg);
void pop(std::string reg);
std::string print();
};  // namespace Asm

void asembly(Node* node);
