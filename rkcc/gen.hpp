#pragma once
#include "ast.hpp"
#include "code.hpp"
#include "symbols.hpp"

class CodeGen {
  Node* root;
  Symbols* symbols;
  Code code;
  std::string cur_func;
  std::string cur_loop;
public:
  CodeGen(Node* root, Symbols* symbols) : root(root), symbols(symbols) {}

  uint16_t gen_gvar(uint16_t global_top);
  uint16_t type_size(Node* node);

  void gen_func();
  // void gen_lvar();
  void gen_stmt(Node* node);
  void gen_expr(Node* node);
  void gen_addr(Node* node);

  void gen_for(Node* node);
  void gen_while(Node* node);
  void gen_break(Node* node);
  void gen_continue(Node* node);
  void gen_goto(Node* node);
  void gen_label(Node* node);


  Code* get_code() { return &code; }
};
