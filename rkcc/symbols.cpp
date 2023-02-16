#include "symbols.hpp"

// LocalSymbols::LocalSymbols(Node* comp_stmt) {
//   for(auto n : comp_stmt->childs) {
//     if(n->type_is(Node::Type::Compound)) {
//       ;
//     }
//   }
// }

GlobalSymbol::GlobalSymbol(Node* node) {
  if(node->type_is(Node::Type::FuncDef)) {
    this->kind = Kind::Func;
    this->name = node->def_name()->str;
    this->type = node->def_type();
    this->body = node->func_body();
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
