#include "utils.hpp"
#include "isa.hpp"

const std::optional<InstructionFormat> get_operation(std::string mnemonic) {
  for(auto itr : isa) {
    if(itr.mnemonic == mnemonic) {
      return itr;
    }
  }
  return std::nullopt;
}

bool is_mnemonic(std::string str) {
  for(auto code : isa) {
    if(code.mnemonic == str) {
      return true;
    }
  }
  return false;
}

uint8_t reg_stoi(std::string name) {
  if(name == "zero") return Reg::zero;
  if(name == "pc") return Reg::pc;
  if(name == "ira") return Reg::ira;
  if(name == "sp") return Reg::sp;
  if(name == "ra") return Reg::ra;
  if(name == "fp") return Reg::fp;
  if(name == "a0") return Reg::a0;
  if(name == "a1") return Reg::a1;
  if(name == "t0") return Reg::t0;
  if(name == "t1") return Reg::t1;
  if(name == "t2") return Reg::t2;
  if(name == "t3") return Reg::t3;
  if(name == "s0") return Reg::s0;
  if(name == "s1") return Reg::s1;
  if(name == "s2") return Reg::s2;
  if(name == "s3") return Reg::s3;
  throw new std::string("Invalid Registor Name: " + name);
  return 0;
}
