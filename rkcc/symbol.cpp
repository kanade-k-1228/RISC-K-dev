#include "symbol.hpp"

std::ostream& operator<<(std::ostream& ss, Symbol s) {
  if(s.kind == Symbol::Kind::Func) ss << "func";
  if(s.kind == Symbol::Kind::GVar) ss << "var";
  if(s.kind == Symbol::Kind::LVar) ss << "var";
  if(s.kind == Symbol::Kind::Arg) ss << "arg";
  if(s.kind == Symbol::Kind::Type) ss << "type";
  return ss;
}
