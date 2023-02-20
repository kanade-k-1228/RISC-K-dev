#include "symbol.hpp"

std::ostream& operator<<(std::ostream& ss, Symbol::Kind kind) {
  if(kind == Symbol::Kind::Func) ss << "func";
  if(kind == Symbol::Kind::GVar) ss << "var";
  if(kind == Symbol::Kind::LVar) ss << "var";
  if(kind == Symbol::Kind::Arg) ss << "arg";
  if(kind == Symbol::Kind::Type) ss << "type";
  return ss;
}
