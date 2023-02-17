#include "symbols.hpp"
#include "dfs.hpp"

LocalSymbols::LocalSymbols() {}
LocalSymbols::LocalSymbols(Node* comp_stmt) {
  DFS dfs(comp_stmt);
  for(Node* n = dfs.begin(); !dfs.end(); n = dfs.next()) {
    if(n->type_is(Node::Type::LVarDef)) {
      LocalSymbol ls;
      ls.name = n->def_name()->str;
      ls.type = n->def_type();
      ls.addr = 0;
      symbols.push_back(ls);
    }
  }
}

GlobalSymbol::GlobalSymbol(Node* node) {
  if(node->type_is(Node::Type::FuncDef)) {
    this->kind = Kind::Func;
    this->name = node->def_name()->str;
    this->type = node->def_type();
    this->body = node->func_body();
    this->ls = LocalSymbols(node->func_body());
  } else if(node->type_is(Node::Type::GVarDef)) {
    this->kind = Kind::GVar;
    this->name = node->def_name()->str;
    this->type = node->def_type();
  } else if(node->type_is(Node::Type::TypeDef)) {
    this->kind = Kind::Type;
    this->name = node->def_name()->str;
    this->type = node->def_type();
  }
}

std::ostream& operator<<(std::ostream& ss, GlobalSymbol gs) {
  if(gs.kind == GlobalSymbol::Kind::Func) ss << "func";
  if(gs.kind == GlobalSymbol::Kind::GVar) ss << "gvar";
  if(gs.kind == GlobalSymbol::Kind::Type) ss << "type";
  return ss;
}


GlobalSymbols::GlobalSymbols(Node* program) {
  for(auto n : program->childs) {
    if(n->type_is(Node::Type::FuncDef) || n->type_is(Node::Type::GVarDef) || n->type_is(Node::Type::TypeDef))
      symbols.emplace_back(n);
  }
}
