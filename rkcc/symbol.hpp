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
    Arg,
    Type,
  };
  Kind kind;
  std::string name;
  Node* type;
  uint16_t addr;
  Node* body;
  Symbols* local;
  Symbol();
  Symbol(Kind kind, std::string name, Node* type)
      : kind(kind), name(name), type(type), addr(0) {}
  Symbol(Kind kind, std::string name, Node* type, Node* body, Symbols* local)
      : kind(kind), name(name), type(type), addr(0), body(body), local(local) {}
  std::string print();
  std::string print_kind();
};
