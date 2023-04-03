#include "imm.hpp"
#include "utils.hpp"
#include <iostream>
Imm::Imm(std::string str) {
  try {
    this->value = std::stoi(str, nullptr, 0);
    this->type = LITERAL;
  } catch(std::out_of_range& e) {
    error("Immidiate Out of Range: " + str);
  } catch(std::invalid_argument& e) {
    this->label = str;
    this->type = LAB_REF;
  }
}
std::string Imm::print() {
  if(type == Imm::LITERAL) return cprint(hex(true, value), YELLOW, 8);
  if(type == Imm::OPR_LAB_REF) return cprint(hex(true, value), GREEN, 8) + cprint(" = " + label, GREEN, 0);
  if(type == Imm::VAR_LAB_REF) return cprint(hex(true, value), BLUE, 8) + cprint(" = " + label, BLUE, 0);
  if(type == Imm::CONST_LAB_REF) return cprint(hex(true, value), YELLOW, 8) + cprint(" = " + label, YELLOW, 0);
  return "";
}
