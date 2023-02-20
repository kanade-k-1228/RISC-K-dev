#include "symbols.hpp"
#include "dfs.hpp"

Symbols::Symbols(){};

// Global
Symbols::Symbols(Node* node) {
  for(auto n : node->childs) {
    if(n->type_is(Node::Type::FuncDef)) {
      symbols.push_back(new Symbol(Symbol::Kind::Func, n->def_name()->str, n->def_type(), n->func_body(), new Symbols(n->def_type(), n->func_body())));
    } else if(n->type_is(Node::Type::GVarDef)) {
      symbols.push_back(new Symbol(Symbol::Kind::GVar, n->def_name()->str, n->def_type()));
    } else if(n->type_is(Node::Type::TypeDef)) {
      symbols.push_back(new Symbol(Symbol::Kind::Type, n->def_name()->str, n->def_type()));
    }
  }
}

// Local
Symbols::Symbols(Node* type, Node* comp_stmt) {
  DFS dfs(comp_stmt);
  // Arg
  for(auto n : type->type_members()) {
    symbols.push_back(new Symbol(Symbol::Kind::Arg, n.first->str, n.second));
  }
  // Var
  for(Node* n = dfs.begin(); !dfs.end(); n = dfs.next()) {
    if(n->type_is(Node::Type::LVarDef)) {
      symbols.push_back(new Symbol(Symbol::Kind::LVar, n->def_name()->str, n->def_type()));
    }
  }
}

Symbol* Symbols::find(std::string name) {
  for(auto s : symbols)
    if(s->name == name) return s;
  return nullptr;
}
