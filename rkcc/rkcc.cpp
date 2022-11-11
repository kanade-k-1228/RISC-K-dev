#include "ast.hpp"
#include "token.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <regex>
#include <string>
#include <vector>


int main(int argc, char* argv[]) {
  // ファイルの読み取り
  if(argc < 1) exit(EXIT_FAILURE);
  std::string fname = argv[1];
  std::ifstream fin(fname, std::fstream::in);
  if(!fin) exit(EXIT_FAILURE);

  // トークン化
  // 先頭から読み取っていく
  const std::regex reserved("^(\\+|-|\\*|/|\\(|\\))");
  const std::regex num("^([0-9]+)");
  const std::regex blank("^(\\s)");
  std::string line;
  std::smatch match;
  Tokens tokens;
  while(std::getline(fin, line)) {
    for(; line.length() > 0;) {
      if(std::regex_search(line, match, reserved)) {
        std::string match_str = match.str();
        int match_length = match.length();
        // 読み取った分だけ読み込みを進める
        line.erase(0, match_length);
        // トークンに追加
        Token new_token(Token::Type::Reserved, match_str, 0);
        tokens.push_back(new_token);
      } else if(std::regex_search(line, match, num)) {
        std::string match_str = match.str();
        int match_length = match.length();
        // 読み取った分だけ読み込みを進める
        line.erase(0, match_length);
        // トークンに追加
        Token new_token(Token::Type::Number, match_str, std::stoi(match_str));
        tokens.push_back(new_token);
      } else if(std::regex_search(line, blank)) {
        // 空白文字を消去
        line.erase(0, 1);
      }
    }

    for(auto token : tokens) std::cout << token.print() << " ";
    std::cout << " --> ";

    // 構文木
    Node* root = expr(tokens);
    std::cout << root->print();
    // 計算結果
    std::cout << " = " << root->reduct() << std::endl;
  }
}
