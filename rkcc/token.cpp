#include "token.hpp"
#include <iostream>

Token::Token(Type type, std::string str, int val)
    : type(type), str(str), val(val) {}

std::string Token::print() {
  if(type == Type::Reserved)
    return "\033[31m" + str + "\033[m";
  else
    return str;
}

bool Tokens::consume_if_token_is(std::string str) {
  if(this->size() > 0 && this->at(0).str == str) {
    this->erase(this->begin());
    return true;
  } else
    return false;
}

bool Tokens::expect_token_is(std::string str) {
  if(this->size() > 0 && this->at(0).str == str) {
    return true;
  } else {
    std::cout << "Parse Error" << std::endl;
    exit(EXIT_FAILURE);
    return false;
  }
}

std::string Tokens::token_head() {
  if(this->size() == 0) return "";
  return this->at(0).str;
}
