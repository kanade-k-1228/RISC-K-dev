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
  // ファイルの読み取り
  if(argc < 1) exit(EXIT_FAILURE);
  std::string fname = argv[1];
  std::ifstream fin(fname, std::fstream::in);
  if(!fin) exit(EXIT_FAILURE);

  // トークン化
  // 先頭から読み取っていく
  std::string line;
  std::smatch match;
  Tokens tokens;
  while(std::getline(fin, line)) {
    try {
      tokenize(line, tokens);
    } catch(std::string e) {
      print_error(fname, 0, line, e);
    }
  }

  // for(auto t : tokens.tokens) std::cout << t;
  // std::cout << std::endl;

  // 構文木の構築
  Node* root = program(tokens);
  // std::cout << root << std::endl;
  // シンボルテーブル生成
  Symbols symbols(root);
  std::cout << "------[Symbols]------" << std::endl;
  for(auto gs : symbols.symbols) {
    std::cout << "+ " << gs->kind
              << " : " << std::setw(8) << std::left << gs->name
              << " : " << gs->type
              << std::endl;
    if(gs->kind == Symbol::Kind::Func) {
      for(auto ls : gs->local->symbols) {
        std::cout << "|  + " << ls->kind << " : " << std::setw(6) << std::left << ls->name
                  << " : " << ls->type << std::endl;
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
