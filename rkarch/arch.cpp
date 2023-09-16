#include "arch.hpp"

const Format& getFormat(std::string mnemonic) {
  for(auto& itr : isa)
    if(itr.mnemonic == mnemonic) return itr;
  throw new std::string("Invalid mnemonic: " + mnemonic);
}

uint8_t get_opcode(uint32_t bin) { return bin & 0xf; }

const Format& getFormat(uint32_t bin) {
  Decoder decoded(bin);
  for(auto& itr : isa)
    if(itr.opc == decoded.opc) return itr;
  throw new std::string("Invalid opecode: " + std::to_string(decoded.opc));
}

bool is_mnemonic(std::string str) {
  for(auto& code : isa)
    if(code.mnemonic == str) return true;
  return false;
}

uint16_t alu(uint8_t code, uint16_t a, uint16_t b) {
  switch(code) {
  case CalcFunc::ADD:
    return a + b;
  case CalcFunc::NOT:
    return !a;
  case CalcFunc::SL:
    return (a << 1);
  case CalcFunc::LROT:
    return ((a << 1) & 0xfffe) | ((a >> 15) & 0x0001);
  case CalcFunc::AND:
    return a & b;
  case CalcFunc::XOR:
    return a ^ b;
  case CalcFunc::OR:
    return a | b;
  case CalcFunc::SUB:
    return a - b;
  case CalcFunc::EQ:
    return a == b ? 0xffff : 0x0000;
  case CalcFunc::NEQ:
    return a != b ? 0xffff : 0x0000;
  case CalcFunc::LTU:
    return ((uint16_t)a < (uint16_t)b) ? 0xffff : 0x0000;
  case CalcFunc::LTS:
    return ((int16_t)a < (int16_t)b) ? 0xffff : 0x0000;
  case CalcFunc::SRU:
    return (a >> 1) & 0x7fff;
  case CalcFunc::SRS:
    return ((a >> 1) & 0x7fff) | (a & 0x8000);
  case CalcFunc::RROT:
    return ((a >> 1) & 0x7fff) | (a << 15 & 0x8000);
  }
  throw "Undefined ALU Code";
}
