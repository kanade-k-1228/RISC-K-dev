#include "imm.hpp"
#include "utils/utils.hpp"
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
