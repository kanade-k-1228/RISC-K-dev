#include "symbols.hpp"
#include "dfs.hpp"

Symbols::Symbols(){};

// Global
Symbols::Symbols(Node* node) {
  for(auto n : node->childs) {
    if(n->type_is(Node::Type::FuncDef)) {
      symbols.emplace_back(Symbol::Kind::Func, n->def_name()->str, n->def_type(), n->func_body(), new Symbols(n->def_type(), n->func_body()));
    } else if(n->type_is(Node::Type::GVarDef)) {
      symbols.emplace_back(Symbol::Kind::GVar, n->def_name()->str, n->def_type());
    } else if(n->type_is(Node::Type::TypeDef)) {
      symbols.emplace_back(Symbol::Kind::Type, n->def_name()->str, n->def_type());
    }
  }
}

// Local
Symbols::Symbols(Node* type, Node* comp_stmt) {
  DFS dfs(comp_stmt);
  // Arg
  for(auto n : type->type_members()) {
    symbols.emplace_back(Symbol::Kind::Arg, n.first->str, n.second);
  }
  // Var
  for(Node* n = dfs.begin(); !dfs.end(); n = dfs.next()) {
    if(n->type_is(Node::Type::LVarDef)) {
      symbols.emplace_back(Symbol::Kind::LVar, n->def_name()->str, n->def_type());
    }
  }
}

Symbol* Symbols::find(std::string name) {
  for(size_t i = 0; i < symbols.size(); ++i) {
    if(symbols.at(i).name == name)
      return &symbols.at(i);
  }
  return nullptr;
}
