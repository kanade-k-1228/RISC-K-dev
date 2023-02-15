#include "ast.hpp"
#include "code.hpp"
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
  while(std::getline(fin, line))
    tokenize(line, tokens);
  for(auto t : tokens.tokens) std::cout << t;
  std::cout << std::endl;

  // 構文木
  Node* root = program(tokens);
  std::cout << root << std::endl;
  // シンボルテーブル生成
  // アセンブラ生成
  // asembly(root);
  // std::cout << Asm::print() << std::endl;
}
