#include "symbol.hpp"
#include "utils.hpp"

std::string Symbol::print_kind() {
  if(kind == Symbol::Kind::Func) return "func";
  if(kind == Symbol::Kind::GVar) return "var";
  if(kind == Symbol::Kind::LVar) return "var";
  if(kind == Symbol::Kind::Arg) return "arg";
  if(kind == Symbol::Kind::Type) return "type";
}

std::string Symbol::print() {
  std::stringstream ss;
  ss << left(this->print_kind(), 4) << " : " << left(this->name, 8) << " : " << this->type;
  return ss.str();
}
