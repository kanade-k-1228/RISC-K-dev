#include "rkisa.hpp"
#include "../utils/utils.hpp"

// // std::string mnemonic_calc = "add|sub|and|or|xor|not|srs|sru|ls|eq|lts|ltu|lcast";
// // std::string mnemonic_calci = "addi|subi|andi|ori|xori|eqi|ltsi|ltui";
// // std::string mnemonic = "add|sub|and|or|xor|not|srs|sru|ls|eq|lts|ltu|lcast|"
// //                        "addi|subi|andi|ori|xori|eqi|ltsi|ltui|"
// //                        "load|store|jmpif|fctrl";
// // std::string mnemonic_pseudo = "loadi|call|ret|iret";


// bool is_calc(std::string s) {
//   return s == "add" || s == "sub"
//          || s == "and" || s == "or" || s == "xor" || s == "not"
//          || s == "srs" || s == "sru" || s == "ls"
//          || s == "lnot" || s == "eq" || s == "lts" || s == "ltu";
// }

// bool is_calci(std::string s) {
//   return s == "addi" || s == "subi"
//          || s == "andi" || s == "ori" || s == "xori"
//          || s == "eqi" || s == "ltsi" || s == "ltui";
// }

// uint16_t str_to_opc(std::string s) {
//   if(is_calc(s)) return OPCode::calc;
//   if(is_calci(s)) return OPCode::calci;
//   if(s == "load") return OPCode::load;
//   if(s == "store") return OPCode::store;
//   if(s == "calif") return OPCode::calif;
//   // Pseudo Mnemonic
//   if(s == "loadi") return CALCI;
//   if(s == "if" || s == "jmp" || s == "call" || s == "ret" || s == "iret") return JMPIF;
//   error("Invalid Operation Name: " + s);
//   return 0;
// }

// uint16_t str_to_func(std::string s) {
//   if(s == "add" || s == "addi") return ADD;
//   if(s == "sub" || s == "subi") return SUB;
//   if(s == "and" || s == "andi") return AND;
//   if(s == "or" || s == "ori") return OR;
//   if(s == "xor" || s == "xori") return XOR;
//   if(s == "not") return NOT;
//   if(s == "srs") return SRS;
//   if(s == "sru") return SRU;
//   if(s == "ls") return SL;
//   if(s == "eq" || s == "eqi") return EQ;
//   if(s == "lts" || s == "ltsi") return LTS;
//   if(s == "ltu" || s == "ltui") return LTU;
//   if(s == "lnot") return LNOT;
//   error("Invalid Operation Name: " + s);
//   return 0;
// }

// OPDecoder::OPDecoder(uint32_t code) {
//   opc = (code >> 12) & 0x000F;
//   func = opc == CALC    ? (code >> 16) & 0x000F
//          : opc == CALCI ? (code >> 4) & 0x000F
//                         : 0;
//   rs1 = (code >> 0) & 0x000F;
//   rs2 = (code >> 4) & 0x000F;
//   rd = (code >> 8) & 0x000F;
//   imm = (code >> 16) & 0xFFFF;
// }

// uint32_t pack(uint8_t u4_0, uint8_t u4_1, uint8_t u4_2, uint8_t u4_3, uint16_t u16) {
//   return (u4_0 & 0x0F)
//          | ((u4_1 & 0x0F) << 4)
//          | ((u4_2 & 0x0F) << 8)
//          | ((u4_3 & 0x0F) << 12)
//          | ((u16 & 0xFFFF) << 16);
// }

// OPEncoder::OPEncoder(uint16_t op, uint16_t func, uint16_t rs1, uint16_t rs2, uint16_t rd, uint16_t imm)
//     : bin(0) {
//   if(op == CALC) {
//     bin = pack(rs1, rs2, rd, op, func);
//   } else if(op == CALCI) {
//     bin = pack(rs1, func, rd, op, imm);
//   } else if(op == LOAD || op == STORE || op == JMPIF) {
//     bin = pack(rs1, rs2, rd, op, imm);
//   } else {
//     error("Invalid Code Format");
//   }
// }
