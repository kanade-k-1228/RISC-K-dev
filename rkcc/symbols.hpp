#pragma once
#include "ast.hpp"
#include <sstream>
#include <string>

// struct Symbol;

// struct Symbols {
//   std::map<std::string, Symbol> symbols;
// };

// struct Symbol {
//   enum class Kind {
//     Func,
//     GVar,
//     LVar,
//     Type,
//   };
//   std::string name;
//   Node* type;
//   uint16_t addr;
//   Node* body;
//   Symbols* local;
// };


struct LocalSymbol {
  std::string name;
  Node* type;
  uint16_t addr;
  LocalSymbol(std::string name, Node* type) : name(name), type(type), addr(0) {}
};

struct LocalSymbols {
  std::vector<LocalSymbol> symbols;
  LocalSymbols();
  LocalSymbols(Node* type, Node* comp_stmt);
};

struct GlobalSymbol {
  enum class Kind {
    Func,
    GVar,
    Type
  };
  Kind kind;
  std::string name;
  Node* type;
  Node* body;
  LocalSymbols ls;
  GlobalSymbol(Node* node);
};

std::ostream& operator<<(std::ostream& ss, GlobalSymbol gs);

struct GlobalSymbols {
  std::vector<GlobalSymbol> symbols;
  GlobalSymbols(Node* program);
  GlobalSymbol* find(std::string name) {
    for(size_t i = 0; i < symbols.size(); ++i) {
      if(symbols.at(i).name == name) {
        return &symbols.at(i);
      }
    }
    return nullptr;
  }
};
