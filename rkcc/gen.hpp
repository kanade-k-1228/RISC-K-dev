#pragma once
#include "ast.hpp"
#include "code.hpp"
#include "symbols.hpp"

class CodeGen {
  Node* root;
  GlobalSymbols* symbols;
  Code code;
public:
  CodeGen(Node* root, GlobalSymbols* symbols) : root(root), symbols(symbols) {}
  uint16_t gen_global(uint16_t global_top);
  void gen_func();
  Code* get_code() { return &code; }
private:
  uint16_t type_size(Node* node);
  void gen_stmt(Node* node);
  void gen_for(Node* node);
  void gen_while(Node* node);
  void gen_break(Node* node);
  void gen_continue(Node* node);
  void gen_goto(Node* node);
  void gen_label(Node* node);

  void gen_expr(Node* node);
};
