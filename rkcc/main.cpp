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
    tokenize(line, tokens);
    for(auto token : tokens) std::cout << token.print() << " ";
    std::cout << " --> ";

    // 構文木
    Node* root = expr(tokens);
    std::cout << print(root);
    // 計算結果
    std::cout << " = " << evaluate(root) << std::endl;
  }
}
