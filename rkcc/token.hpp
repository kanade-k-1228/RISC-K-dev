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
  bool type_is(Type type) { return this->type == type; }
  bool str_is(std::string str) { return this->str == str; };
  std::string get_str() { return str; }
  int get_val() { return val; }
  std::string print();
};

namespace TokenRegex {
const std::regex blank("^\\s");
const std::regex sign("^("
                      "(=>)|(\\|\\|)|(\\^\\^)|(&&)|(==)|(!=)|(<=)|(>=)|(<<)|(>>)|(\\+\\+)|(--)"
                      "|@|\\?|:|\\||\\^|&|<|>|\\+|-|\\*|/|%|\\(|\\)|\\[|\\]|\\{|\\}|;|,|\\.|="
                      ")");
const std::regex ident("^([_a-zA-Z][_0-9a-zA-Z]*)");
const std::regex number("^([0-9]+)");
}  // namespace TokenRegex

class Tokens {
public:
  std::vector<Token> tokens;
  uint current = 0;

  void push_back(Token token) { tokens.push_back(token); }
  Token head() { return tokens.at(current); }           // 先頭のトークン
  Token head(int n) { return tokens.at(current + n); }  // 先頭からn番目のトークン
  Token pop() { return tokens.at(current++); }          // 先頭のトークンを読み進める
  bool consume(std::string str) {                       // 先頭が一致した場合のみ読み進める
    if(head().str_is(str)) {
      pop();
      return true;
    } else {
      return false;
    }
  }
  bool empty() { return tokens.size() <= current; }  // 最後まで到達したか

  // 対応する閉じ括弧の相対位置を探索
  int pair_bracket(int nest_cnt, std::string open, std::string close) {
    for(int i = 0; i + current < tokens.size(); ++i) {
      if(head(i).str_is(open)) ++nest_cnt;
      if(head(i).str_is(close)) --nest_cnt;
      if(nest_cnt == 0) return i;
    }
    return -1;
  }
};

void tokenize(std::string, Tokens&);
