#include "imm.hpp"
#include "../utils.hpp"
#include <iostream>

Imm::Imm(std::string str) {
  try {
    this->value = std::stoi(str, nullptr, 0);
    this->type = LITERAL;
  } catch(std::out_of_range& e) {
    throw new std::string("Immidiate Out of Range: " + str);
  } catch(std::invalid_argument& e) {
    this->label = str;
    this->type = LAB_REF;
  }
}

std::string Imm::print_pretty() {
  if(type == Imm::LITERAL) return yellow(hex(value));
  if(type == Imm::OPR_LAB_REF) return green(hex(value) + " = " + label);
  if(type == Imm::VAR_LAB_REF) return cyan(hex(value) + " = " + label);
  if(type == Imm::CONST_LAB_REF) return yellow(hex(value) + " = " + label);
  return "";
}

std::string Imm::print_format() { return label; }
