#include "rkisa.hpp"
#include "../utils/utils.hpp"

std::string mnemonic = "add|sub|and|or|xor|not|srs|sru|ls|lnot|eq|lts|ltu|"
                       "addi|subi|andi|ori|xori|eqi|ltsi|ltui|"
                       "store|load|callif";
std::string pseudo_mnemonic = "loadi|if|jmp|call|ret|iret";

uint16_t str_to_reg(std::string name) {
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

bool is_calc(std::string s) {
  return s == "add" || s == "sub"
         || s == "and" || s == "or" || s == "xor" || s == "not"
         || s == "srs" || s == "sru" || s == "ls"
         || s == "lnot" || s == "eq" || s == "lts" || s == "ltu";
}

bool is_calci(std::string s) {
  return s == "addi" || s == "subi"
         || s == "andi" || s == "ori" || s == "xori"
         || s == "eqi" || s == "ltsi" || s == "ltui";
}

uint16_t str_to_opc(std::string s) {
  if(is_calc(s)) return CALC;
  if(is_calci(s)) return CALCI;
  if(s == "load") return LOAD;
  if(s == "store") return STORE;
  if(s == "callif") return JMPIF;
  // Pseudo Mnemonic
  if(s == "loadi") return CALCI;
  if(s == "if" || s == "jmp" || s == "call" || s == "ret" || s == "iret") return JMPIF;
  error("Invalid Operation Name: " + s);
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
  if(s == "ls") return SL;
  if(s == "eq" || s == "eqi") return EQ;
  if(s == "lts" || s == "ltsi") return LTS;
  if(s == "ltu" || s == "ltui") return LTU;
  if(s == "lnot") return LNOT;
  error("Invalid Operation Name: " + s);
  return 0;
}

OPDecoder::OPDecoder(uint32_t code) {
  opc = (code >> 12) & 0x000F;
  func = opc == CALC    ? (code >> 16) & 0x000F
         : opc == CALCI ? (code >> 4) & 0x000F
                        : 0;
  rs1 = (code >> 0) & 0x000F;
  rs2 = (code >> 4) & 0x000F;
  rd = (code >> 8) & 0x000F;
  imm = (code >> 16) & 0xFFFF;
}

uint32_t pack(uint8_t u4_0, uint8_t u4_1, uint8_t u4_2, uint8_t u4_3, uint16_t u16) {
  return (u4_0 & 0x0F)
         | ((u4_1 & 0x0F) << 4)
         | ((u4_2 & 0x0F) << 8)
         | ((u4_3 & 0x0F) << 12)
         | ((u16 & 0xFFFF) << 16);
}

OPEncoder::OPEncoder(uint16_t op, uint16_t func, uint16_t rs1, uint16_t rs2, uint16_t rd, uint16_t imm)
    : bin(0) {
  if(op == CALC) {
    bin = pack(rs1, rs2, rd, op, func);
  } else if(op == CALCI) {
    bin = pack(rs1, func, rd, op, imm);
  } else if(op == LOAD || op == STORE || op == JMPIF) {
    bin = pack(rs1, rs2, rd, op, imm);
  } else {
    error("Invalid Code Format");
  }
}
