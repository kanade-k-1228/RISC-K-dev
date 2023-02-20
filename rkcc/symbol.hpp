#pragma once
#include "ast.hpp"
#include "symbols.hpp"
#include <string>

struct Symbols;

struct Symbol {
  enum class Kind {
    Func,
    GVar,
    LVar,
    Type,
  };
  std::string name;
  Node* type;
  uint16_t addr;
  Node* body;
  Symbols* local;
};
