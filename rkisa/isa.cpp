#include "isa.hpp"

const Format& getFormat(std::string mnemonic) {
  for(auto& itr : isa)
    if(itr.mnemonic == mnemonic) return itr;
  throw new std::string("Invalid mnemonic" + mnemonic);
}

bool is_mnemonic(std::string str) {
  for(auto& code : isa)
    if(code.mnemonic == str) return true;
  return false;
}
