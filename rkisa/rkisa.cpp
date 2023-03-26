#include "rkisa.hpp"
#include "../utils/utils.hpp"

uint16_t decode_func(uint32_t code, uint16_t opc) {
  if(opc == CALC) return (code >> 10) & 0x000f;
  if(opc == CALCI) return (code >> 20) & 0x000f;
  return 0;
}

uint16_t decode_imm(uint32_t code, uint16_t opc) {
  if(opc == CALC) return 0;
  if(opc == CALCI) return ((code >> 10) & 0x03ff) | (((code >> 24) & 0x0003) << 10);
  if(opc == LOAD || opc == LOADI || opc == JUMP) return (code >> 10) & 0xffff;
  if(opc == STORE || opc == BREQ || opc == BRLT) return ((code >> 10) & 0x03ff) | ((code >> 26) & 0x003f);
  error("Unknown OPC: " + opc);
  return 0;
}

OPDecoder::OPDecoder(uint32_t code) {
  opc = (code >> 6) & 0x000f;
  func = decode_func(code, opc);
  rs1 = (code >> 0) & 0x003f;
  rs2 = (code >> 20) & 0x003f;
  rd = (code >> 26) & 0x003f;
  imm = decode_imm(code, opc);
}

OPEncoder::OPEncoder(uint16_t op, uint16_t func, uint16_t rs1, uint16_t rs2, uint16_t rd, uint16_t imm) {
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
    bin |= (imm & 0x3FF) << 10;
    bin |= (func & 0x0F) << 20;
    bin |= (imm & 0xA00) >> 10 << 24;
    bin |= (rd & 0x3F) << 26;
  } else if(op == LOAD || op == LOADI || op == JUMP) {
    bin |= (rs1 & 0x3F);
    bin |= (op & 0x0F) << 6;
    bin |= (imm & 0xFFFF) << 10;
    bin |= (rd & 0x3F) << 26;
  } else if(op == STORE || op == BREQ || op == BRLT) {
    bin |= (rs1 & 0x3F);
    bin |= (op & 0x0F) << 6;
    bin |= (imm & 0x3FF) << 10;
    bin |= (rs2 & 0x3F) << 20;
    bin |= (imm & 0xFA00) >> 10 << 26;
  } else {
    error("Invalid Code Format");
  }
}