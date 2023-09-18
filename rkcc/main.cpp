#include "ast.hpp"
#include "code.hpp"
#include "dfs.hpp"
#include "gen.hpp"
#include "symbols.hpp"
#include "token.hpp"
#include "type.hpp"
#include "utils.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

int main(int argc, char* argv[]) {

  bool print_token = true;

  // ファイルの読み取り
  if(argc < 1) exit(EXIT_FAILURE);
  std::string fname = argv[1];
  std::ifstream fin(fname, std::fstream::in);
  if(!fin) exit(EXIT_FAILURE);

  // トークン化
  std::smatch match;
  Tokens tokens;
  std::string line;
  int line_cnt = 0;
  while(std::getline(fin, line)) {
    line_cnt++;
    try {
      tokenize(line, tokens);
    } catch(std::string e) {
      print_error(fname, line_cnt, line, e);
    }
  }

  if(print_token) {
    std::cout << "------[Tokens]------" << std::endl;
    for(auto t : tokens.tokens) std::cout << t.print() << " ";
    std::cout << std::endl;
  }

  // 構文木の構築
  Node* root;
  try {
    root = program(tokens);
  } catch(std::string e) {
    print_error(fname, 0, "", e);
  }

  // std::cout << root << std::endl;
  // シンボルテーブル生成
  Symbols symbols(root);
  std::cout << "------[Symbols]------" << std::endl;
  for(auto gs : symbols.symbols) {
    std::cout << "+ " << gs->print() << std::endl;
    if(gs->kind == Symbol::Kind::Func) {
      for(auto ls : gs->local->symbols) {
        std::cout << "|  + " << gs->print() << std::endl;
      }
    }
  }
  std::cout << std::endl;
  // アセンブラ生成
  CodeGen gen(root, &symbols);
  gen.gen_gvar(0x1000);
  gen.gen_func();
  std::cout << "------[ASM Out]------" << std::endl
            << *gen.get_code()
            << "---------------------" << std::endl;
}
