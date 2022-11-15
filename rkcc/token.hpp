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
const std::regex ident("^([_a-zA-Z][_0-9a-zA-Z]*)");
const std::regex number("^([0-9]+)");
}  // namespace TokenRegex

class Tokens : public std::vector<Token> {
public:
  bool consume(std::string);
  bool consume(Token::Type);
  bool type_is(Token::Type);
  bool expect(std::string);
  void pop();
  std::string token_head();
};

void tokenize(std::string, Tokens&);
