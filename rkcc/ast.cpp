#include "ast.hpp"

Node::Node(Type type, Node* cond, Node* lhs, Node* rhs)
    : type(type), lhs(lhs), rhs(rhs), cond(cond), val(0) {}

Node::Node(Type type, Node* lhs, Node* rhs)
    : type(type), lhs(lhs), rhs(rhs), cond(nullptr), val(0) {}

Node::Node(Type type, Node* lhs)
    : type(type), lhs(lhs), rhs(nullptr), cond(nullptr), val(0) {}

Node::Node(int val)
    : type(Type::Num), lhs(nullptr), rhs(nullptr), cond(nullptr), val(val) {}

std::string Node::print() {
  if(type == Type::Cond) return "( " + cond->print() + " ? " + lhs->print() + " : " + rhs->print() + " )";
  if(type == Type::LogicalOr) return "( " + lhs->print() + " || " + rhs->print() + " )";
  if(type == Type::LogicalXor) return "( " + lhs->print() + " ^^ " + rhs->print() + " )";
  if(type == Type::LogicalAnd) return "( " + lhs->print() + " && " + rhs->print() + " )";
  if(type == Type::BitOr) return "( " + lhs->print() + " | " + rhs->print() + " )";
  if(type == Type::BitXor) return "( " + lhs->print() + " ^ " + rhs->print() + " )";
  if(type == Type::BitAnd) return "( " + lhs->print() + " & " + rhs->print() + " )";
  if(type == Type::EQ) return "( " + lhs->print() + " == " + rhs->print() + " )";
  if(type == Type::NEQ) return "( " + lhs->print() + " != " + rhs->print() + " )";
  if(type == Type::LT) return "( " + lhs->print() + " < " + rhs->print() + " )";
  if(type == Type::LEQ) return "( " + lhs->print() + " <= " + rhs->print() + " )";
  if(type == Type::GT) return "( " + lhs->print() + " > " + rhs->print() + " )";
  if(type == Type::GEQ) return "( " + lhs->print() + " >= " + rhs->print() + " )";
  if(type == Type::RShift) return "( " + lhs->print() + " >> " + rhs->print() + " )";
  if(type == Type::LShift) return "( " + lhs->print() + " << " + rhs->print() + " )";
  if(type == Type::Add) return "( " + lhs->print() + " + " + rhs->print() + " )";
  if(type == Type::Sub) return "( " + lhs->print() + " - " + rhs->print() + " )";
  if(type == Type::Mul) return "( " + lhs->print() + " * " + rhs->print() + " )";
  if(type == Type::Div) return "( " + lhs->print() + " / " + rhs->print() + " )";
  if(type == Type::Mod) return "( " + lhs->print() + " % " + rhs->print() + " )";
  if(type == Type::UnaryAdd) return "( ++ " + lhs->print() + " )";
  if(type == Type::UnarySub) return "( -- " + lhs->print() + " )";
  if(type == Type::PostAdd) return "( " + lhs->print() + " ++ )";
  if(type == Type::PostSub) return "( " + lhs->print() + " -- )";
  if(type == Type::Num) return std::to_string(val);
  return "";
}

int Node::evaluate() {
  if(type == Type::Cond) return cond->evaluate() ? lhs->evaluate() : rhs->evaluate();
  if(type == Type::LogicalOr) return lhs->evaluate() | rhs->evaluate();
  if(type == Type::LogicalXor) return lhs->evaluate() ^ rhs->evaluate();
  if(type == Type::LogicalAnd) return lhs->evaluate() & rhs->evaluate();
  if(type == Type::BitOr) return lhs->evaluate() | rhs->evaluate();
  if(type == Type::BitXor) return lhs->evaluate() ^ rhs->evaluate();
  if(type == Type::BitAnd) return lhs->evaluate() & rhs->evaluate();
  if(type == Type::EQ) return lhs->evaluate() == rhs->evaluate();
  if(type == Type::NEQ) return lhs->evaluate() != rhs->evaluate();
  if(type == Type::LT) return lhs->evaluate() < rhs->evaluate();
  if(type == Type::LEQ) return lhs->evaluate() <= rhs->evaluate();
  if(type == Type::GT) return lhs->evaluate() > rhs->evaluate();
  if(type == Type::GEQ) return lhs->evaluate() >= rhs->evaluate();
  if(type == Type::RShift) return lhs->evaluate() >> rhs->evaluate();
  if(type == Type::LShift) return lhs->evaluate() << rhs->evaluate();
  if(type == Type::Add) return lhs->evaluate() + rhs->evaluate();
  if(type == Type::Sub) return lhs->evaluate() - rhs->evaluate();
  if(type == Type::Mul) return lhs->evaluate() * rhs->evaluate();
  if(type == Type::Div) return lhs->evaluate() / rhs->evaluate();
  if(type == Type::Mod) return lhs->evaluate() % rhs->evaluate();
  if(type == Type::UnaryAdd) return lhs->evaluate() + 1;
  if(type == Type::UnarySub) return lhs->evaluate() - 1;
  if(type == Type::PostAdd) return lhs->evaluate() + 1;
  if(type == Type::PostSub) return lhs->evaluate() - 1;
  if(type == Type::Num) return val;
  return 0;
}

Node* expr(Tokens& tokens) {
  Node* node = cond(tokens);
  return node;
}

Node* cond(Tokens& tokens) {
  Node* node = logical_or(tokens);
  if(tokens.consume("?")) {
    Node* true_expr = expr(tokens);
    tokens.expect(":");
    Node* false_expr = expr(tokens);
    node = new Node(Node::Type::Cond, node, true_expr, false_expr);
    return node;
  } else {
    return node;
  }
}

Node* logical_or(Tokens& tokens) {
  Node* node = logical_xor(tokens);
  for(;;) {
    if(tokens.consume("||"))
      node = new Node(Node::Type::LogicalOr, node, logical_xor(tokens));
    else
      return node;
  }
}

Node* logical_xor(Tokens& tokens) {
  Node* node = logical_and(tokens);
  for(;;) {
    if(tokens.consume("^^"))
      node = new Node(Node::Type::LogicalXor, node, logical_and(tokens));
    else
      return node;
  }
}

Node* logical_and(Tokens& tokens) {
  Node* node = bit_or(tokens);
  for(;;) {
    if(tokens.consume("&&"))
      node = new Node(Node::Type::LogicalAnd, node, bit_or(tokens));
    else
      return node;
  }
}

Node* bit_or(Tokens& tokens) {
  Node* node = bit_xor(tokens);
  for(;;) {
    if(tokens.consume("|"))
      node = new Node(Node::Type::BitOr, node, bit_xor(tokens));
    else
      return node;
  }
}

Node* bit_xor(Tokens& tokens) {
  Node* node = bit_and(tokens);
  for(;;) {
    if(tokens.consume("^"))
      node = new Node(Node::Type::BitXor, node, bit_and(tokens));
    else
      return node;
  }
}

Node* bit_and(Tokens& tokens) {
  Node* node = equality(tokens);
  for(;;) {
    if(tokens.consume("&"))
      node = new Node(Node::Type::BitAnd, node, equality(tokens));
    else
      return node;
  }
}

Node* equality(Tokens& tokens) {
  Node* node = relation(tokens);
  if(tokens.consume("=="))
    node = new Node(Node::Type::EQ, node, relation(tokens));
  else if(tokens.consume("!="))
    node = new Node(Node::Type::NEQ, node, relation(tokens));
  return node;
}

Node* relation(Tokens& tokens) {
  Node* node = shift(tokens);
  if(tokens.consume("<"))
    node = new Node(Node::Type::LT, node, shift(tokens));
  else if(tokens.consume("<="))
    node = new Node(Node::Type::LEQ, node, shift(tokens));
  else if(tokens.consume(">"))
    node = new Node(Node::Type::GT, node, shift(tokens));
  else if(tokens.consume(">="))
    node = new Node(Node::Type::GEQ, node, shift(tokens));
  return node;
}

Node* shift(Tokens& tokens) {
  Node* node = add(tokens);
  for(;;) {
    if(tokens.consume("<<"))
      node = new Node(Node::Type::LShift, node, add(tokens));
    else if(tokens.consume(">>"))
      node = new Node(Node::Type::RShift, node, add(tokens));
    else
      return node;
  }
}

Node* add(Tokens& tokens) {
  Node* node = mul(tokens);
  for(;;) {
    if(tokens.consume("+"))
      node = new Node(Node::Type::Add, node, mul(tokens));
    else if(tokens.consume("-"))
      node = new Node(Node::Type::Sub, node, mul(tokens));
    else
      return node;
  }
}

Node* mul(Tokens& tokens) {
  Node* node = unary(tokens);
  for(;;) {
    if(tokens.consume("*"))
      node = new Node(Node::Type::Mul, node, unary(tokens));
    else if(tokens.consume("/"))
      node = new Node(Node::Type::Div, node, unary(tokens));
    else if(tokens.consume("%"))
      node = new Node(Node::Type::Mod, node, unary(tokens));
    else
      return node;
  }
}

Node* unary(Tokens& tokens) {
  if(tokens.consume("++"))
    return new Node(Node::Type::UnaryAdd, post(tokens));
  else if(tokens.consume("--"))
    return new Node(Node::Type::UnarySub, post(tokens));
  else
    return post(tokens);
}

Node* post(Tokens& tokens) {
  Node* node = prim(tokens);
  if(tokens.consume("++"))
    return new Node(Node::Type::PostAdd, node);
  else if(tokens.consume("--"))
    return new Node(Node::Type::PostSub, node);
  else
    return node;
}

Node* prim(Tokens& tokens) {
  if(tokens.consume("(")) {
    Node* node = expr(tokens);
    tokens.expect(")");
    return node;
  } else {
    Node* node = new Node(tokens.at(0).val);
    tokens.erase(tokens.begin());
    return node;
  }
}
