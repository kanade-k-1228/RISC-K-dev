#pragma once
#include <string>
#include <vector>

class Token {
public:
  enum class Type {
    Reserved,
    OperatorAdd,   // +
    OperatorSub,   // -
    ParOpen,       // (
    ParClose,      // )
    BracketOpen,   //
    BracketClose,  //
    CurlyOpen,     // {
    CurlyClose,    // }
    AngleOpen,     // <
    AngleClose,    //
    Number,
    Identifier,
  };

  Type type;
  std::string str;
  int val;
  Token(Type type, std::string str, int val);
  std::string print();
};

class Tokens : public std::vector<Token> {
public:
  bool consume_if_token_is(std::string);
  bool expect_token_is(std::string);
  std::string token_head();
};
