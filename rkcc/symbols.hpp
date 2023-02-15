#include "ast.hpp"
#include <string>

struct Symbol {
  enum class Type {
    Func,
    GVar,
    LVar,
    Type
  };
  std::string name;
  Node* type;
  Node* body;
  std::string mem_type;
  int addr;
};

struct Symbols {
  std::vector<Symbol> symbols;
  Symbol* find(std::string name) {
    for(auto s : symbols) {
      if(s.name == name) return &s;
    }
    return nullptr;
  }
};
