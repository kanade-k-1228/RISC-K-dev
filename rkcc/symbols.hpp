#pragma once
#include "ast.hpp"
#include "dfs.hpp"
#include "symbol.hpp"
#include <sstream>
#include <string>

struct Symbol;

struct Symbols {
  std::vector<Symbol*> symbols;
  Symbols();
  Symbols(Node* node);                   // Global
  Symbols(Node* type, Node* comp_stmt);  // Local
  Symbol* find(std::string name);
};
