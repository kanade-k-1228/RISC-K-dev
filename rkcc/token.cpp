#include "token.hpp"
#include "utils.hpp"
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
    error("Parse Error");
    return false;
  }
}

std::string Tokens::token_head() {
  if(this->size() == 0) return "";
  return this->at(0).str;
}

void tokenize(std::string code, Tokens& tokens) {
  std::smatch match;
  for(; code.length() > 0;) {
    if(std::regex_search(code, match, TokenRegex::blank)) {
      // 空白文字を消去
      code.erase(0, 1);
    } else if(std::regex_search(code, match, TokenRegex::reserved)) {
      std::string match_str = match.str();
      int match_length = match.length();
      // 読み取った分だけ読み込みを進める
      code.erase(0, match_length);
      // トークンに追加
      Token new_token(Token::Type::Reserved, match_str, 0);
      tokens.push_back(new_token);
    } else if(std::regex_search(code, match, TokenRegex::number)) {
      std::string match_str = match.str();
      int match_length = match.length();
      // 読み取った分だけ読み込みを進める
      code.erase(0, match_length);
      // トークンに追加
      Token new_token(Token::Type::Number, match_str, std::stoi(match_str));
      tokens.push_back(new_token);
    } else {
      error("Undefined Token: " + code);
    }
  }
}