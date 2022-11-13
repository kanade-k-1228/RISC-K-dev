#pragma once
#include <regex>
#include <string>
#include <vector>

class Token {
public:
  enum class Type {
    Reserved,
    Identifier,
    Number,
  };

  Type type;
  std::string str;
  int val;
  Token(Type type, std::string str, int val);
  std::string print();
};

namespace TokenRegex {
const std::regex blank("^\\s");
const std::regex reserved("^("
                          "(if)|(goto)|(while)|(do)|(for)"
                          "|(\\|\\|)|(\\^\\^)|(&&)|(==)|(!=)|(<=)|(>=)|(<<)|(>>)|(\\+\\+)|(--)"
                          "|\\?|:|\\||\\^|&|<|>|\\+|-|\\*|/|%|\\(|\\)"
                          ")");
const std::regex number("^([0-9]+)");
}  // namespace TokenRegex

class Tokens : public std::vector<Token> {
public:
  bool consume(std::string);
  bool expect(std::string);
  std::string token_head();
};

void tokenize(std::string, Tokens&);
