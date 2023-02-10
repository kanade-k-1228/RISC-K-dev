#include "token.hpp"
#include "../utils/utils.hpp"
#include <iostream>

Token::Token(Type type, std::string str, int val)
    : type(type), str(str), val(val) {}

bool Token::type_is(Type type) { return this->type == type; }

std::string Token::print() {
  std::stringstream ss;
  if(type == Type::Sign) ss << "\033[31m" << str << "\033[m ";
  if(type == Type::Identifier) ss << "\033[34m" << str << "\033[m ";
  if(type == Type::Number) ss << "\033[32m" << val << "\033[m ";
  return ss.str();
}

void tokenize(std::string code, Tokens& tokens) {
  std::smatch match;
  for(; code.length() > 0;) {
    if(std::regex_search(code, match, TokenRegex::blank)) {
      // 空白文字を消去
      code.erase(0, 1);
    } else if(std::regex_search(code, match, TokenRegex::sign)) {
      std::string match_str = match.str();
      int match_length = match.length();
      // 読み取った分だけ読み込みを進める
      code.erase(0, match_length);
      // トークンに追加
      Token new_token(Token::Type::Sign, match_str, 0);
      tokens.push_back(new_token);
    } else if(std::regex_search(code, match, TokenRegex::ident)) {
      std::string match_str = match.str();
      int match_length = match.length();
      // 読み取った分だけ読み込みを進める
      code.erase(0, match_length);
      // トークンに追加
      Token new_token(Token::Type::Identifier, match_str, 0);
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
