#include "ast.hpp"

Node::Node(Type type, Node* lhs, Node* rhs)
    : type(type), lhs(lhs), rhs(rhs) {}

Node::Node(int val)
    : type(Type::Num), val(val) {}


std::string Node::print() {
  if(lhs && rhs) {
    std::string op;
    if(type == Type::Add) op = " + ";
    if(type == Type::Sub) op = " - ";
    if(type == Type::Mul) op = " * ";
    if(type == Type::Div) op = " / ";
    return "( " + lhs->print() + op + rhs->print() + " )";
  } else {
    return std::to_string(val);
  }
}

int Node::evaluate() {
  if(type == Type::Add) return lhs->evaluate() + rhs->evaluate();
  if(type == Type::Sub) return lhs->evaluate() - rhs->evaluate();
  if(type == Type::Mul) return lhs->evaluate() * rhs->evaluate();
  if(type == Type::Div) return lhs->evaluate() / rhs->evaluate();
  if(type == Type::Num) return val;
  exit(EXIT_FAILURE);
  return 0;
}

Node* expr(Tokens& tokens) {
  //   std::cout << "Expr    " << token_head() << std::endl;
  Node* node = mul(tokens);
  for(;;) {
    if(tokens.consume_if_token_is("+")) {
      node = new Node(Node::Type::Add, node, mul(tokens));
    } else if(tokens.consume_if_token_is("-")) {
      node = new Node(Node::Type::Sub, node, mul(tokens));
    } else
      return node;
  }
}

Node* mul(Tokens& tokens) {
  //   std::cout << "Mul     " << token_head() << std::endl;
  Node* node = primary(tokens);
  for(;;) {
    if(tokens.consume_if_token_is("*")) {
      node = new Node(Node::Type::Mul, node, primary(tokens));
    } else if(tokens.consume_if_token_is("/")) {
      node = new Node(Node::Type::Div, node, primary(tokens));
    } else
      return node;
  }
}

Node* primary(Tokens& tokens) {
  //   std::cout << "Primary " << token_head() << std::endl;
  if(tokens.consume_if_token_is("(")) {
    Node* node = expr(tokens);
    tokens.erase(tokens.begin());
    return node;
  } else {
    Node* node = new Node(tokens.at(0).val);
    tokens.erase(tokens.begin());
    return node;
  }
}
