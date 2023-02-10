#pragma once
#include <regex>
#include <string>
#include <vector>

class Token {
public:
  enum class Type {
    Sign,
    Identifier,
    Number,
  };
  Type type;
  std::string str;
  int val;
  Token(Type type, std::string str, int val);
  bool type_is(Type);
  std::string print();
};

namespace TokenRegex {
const std::regex blank("^\\s");
const std::regex sign("^("
                      "(=>)|(\\|\\|)|(\\^\\^)|(&&)|(==)|(!=)|(<=)|(>=)|(<<)|(>>)|(\\+\\+)|(--)"
                      "|@|\\?|:|\\||\\^|&|<|>|\\+|-|\\*|/|%|\\(|\\)|\\{|\\}|;|,|="
                      ")");
const std::regex ident("^([_a-zA-Z][_0-9a-zA-Z]*)");
const std::regex number("^([0-9]+)");
}  // namespace TokenRegex

class Tokens {
public:
  std::vector<Token> tokens;
  uint current = 0;

  void push_back(Token token) { tokens.push_back(token); }
  Token head() { return tokens.at(current); }                    // 先頭のトークン
  Token pop() { return tokens.at(current++); }                   // 先頭のトークンを読み進める
  bool head_is(std::string str) { return (head().str == str); }  // 先頭が一致するか
  bool consume(std::string str) {                                // 先頭が一致した場合のみ読み進める
    if(head_is(str)) {
      pop();
      return true;
    } else {
      return false;
    }
  }
  bool empty() { return tokens.size() <= current; }  // 最後まで到達したか
};

void tokenize(std::string, Tokens&);
