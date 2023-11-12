#include "imm.hpp"
#include "utils/utils.hpp"
#include <iostream>

Imm::Imm(std::string str) {
  auto ret = stoios(str);
  if(std::holds_alternative<int>(ret)) {
    this->value = std::get<int>(ret);
    this->type = LITERAL;
  }
  if(std::holds_alternative<std::string>(ret)) {
    this->label = std::get<std::string>(ret);
    this->type = LAB_REF;
  }
}
