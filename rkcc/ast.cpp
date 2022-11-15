#include "ast.hpp"
#include <string>
#include <vector>

Node::Node(Type type, Node* cond, Node* lhs, Node* rhs)
    : type(type), lhs(lhs), rhs(rhs), cond(cond), val(0) {}

Node::Node(Type type, Node* lhs, Node* rhs)
    : type(type), lhs(lhs), rhs(rhs), cond(nullptr), val(0) {}

Node::Node(Type type, Node* lhs)
    : type(type), lhs(lhs), rhs(nullptr), cond(nullptr), val(0) {}

Node::Node(int val)
    : type(Type::Num), lhs(nullptr), rhs(nullptr), cond(nullptr), val(val) {}

Node::Node(std::string ident)
    : type(Type::Ident), lhs(nullptr), rhs(nullptr), cond(nullptr), str(ident) {}

std::string print(Node* node) {
  if(node->type == Node::Type::Cond) return "( " + print(node->cond) + " ? " + print(node->lhs) + " : " + print(node->rhs) + " )";
  if(node->type == Node::Type::LogicalOr) return "( " + print(node->lhs) + " || " + print(node->rhs) + " )";
  if(node->type == Node::Type::LogicalXor) return "( " + print(node->lhs) + " ^^ " + print(node->rhs) + " )";
  if(node->type == Node::Type::LogicalAnd) return "( " + print(node->lhs) + " && " + print(node->rhs) + " )";
  if(node->type == Node::Type::BitOr) return "( " + print(node->lhs) + " | " + print(node->rhs) + " )";
  if(node->type == Node::Type::BitXor) return "( " + print(node->lhs) + " ^ " + print(node->rhs) + " )";
  if(node->type == Node::Type::BitAnd) return "( " + print(node->lhs) + " & " + print(node->rhs) + " )";
  if(node->type == Node::Type::EQ) return "( " + print(node->lhs) + " == " + print(node->rhs) + " )";
  if(node->type == Node::Type::NEQ) return "( " + print(node->lhs) + " != " + print(node->rhs) + " )";
  if(node->type == Node::Type::LT) return "( " + print(node->lhs) + " < " + print(node->rhs) + " )";
  if(node->type == Node::Type::LEQ) return "( " + print(node->lhs) + " <= " + print(node->rhs) + " )";
  if(node->type == Node::Type::GT) return "( " + print(node->lhs) + " > " + print(node->rhs) + " )";
  if(node->type == Node::Type::GEQ) return "( " + print(node->lhs) + " >= " + print(node->rhs) + " )";
  if(node->type == Node::Type::RShift) return "( " + print(node->lhs) + " >> " + print(node->rhs) + " )";
  if(node->type == Node::Type::LShift) return "( " + print(node->lhs) + " << " + print(node->rhs) + " )";
  if(node->type == Node::Type::Add) return "( " + print(node->lhs) + " + " + print(node->rhs) + " )";
  if(node->type == Node::Type::Sub) return "( " + print(node->lhs) + " - " + print(node->rhs) + " )";
  if(node->type == Node::Type::Mul) return "( " + print(node->lhs) + " * " + print(node->rhs) + " )";
  if(node->type == Node::Type::Div) return "( " + print(node->lhs) + " / " + print(node->rhs) + " )";
  if(node->type == Node::Type::Mod) return "( " + print(node->lhs) + " % " + print(node->rhs) + " )";
  if(node->type == Node::Type::UnaryAdd) return "( ++ " + print(node->lhs) + " )";
  if(node->type == Node::Type::UnarySub) return "( -- " + print(node->lhs) + " )";
  if(node->type == Node::Type::PostAdd) return "( " + print(node->lhs) + " ++ )";
  if(node->type == Node::Type::PostSub) return "( " + print(node->lhs) + " -- )";
  if(node->type == Node::Type::Num) return std::to_string(node->val);
  if(node->type == Node::Type::Ident) return node->str;
  return "";
}

std::vector<std::string> print_tree(Node* node) {
  std::vector<std::string> ret;
  if(node->type == Node::Type::Num) {
    ret.at(0) = std::to_string(node->val);
    return ret;
  }
}

// 1 + 2 + 3 + 4
// └─+─┘   │   │
//   └──+──┘   │
//      └──+───┘

int evaluate(Node* node) {
  if(node->type == Node::Type::Cond) return evaluate(node->cond) ? evaluate(node->lhs) : evaluate(node->rhs);
  if(node->type == Node::Type::LogicalOr) return evaluate(node->lhs) | evaluate(node->rhs);
  if(node->type == Node::Type::LogicalXor) return evaluate(node->lhs) ^ evaluate(node->rhs);
  if(node->type == Node::Type::LogicalAnd) return evaluate(node->lhs) & evaluate(node->rhs);
  if(node->type == Node::Type::BitOr) return evaluate(node->lhs) | evaluate(node->rhs);
  if(node->type == Node::Type::BitXor) return evaluate(node->lhs) ^ evaluate(node->rhs);
  if(node->type == Node::Type::BitAnd) return evaluate(node->lhs) & evaluate(node->rhs);
  if(node->type == Node::Type::EQ) return evaluate(node->lhs) == evaluate(node->rhs);
  if(node->type == Node::Type::NEQ) return evaluate(node->lhs) != evaluate(node->rhs);
  if(node->type == Node::Type::LT) return evaluate(node->lhs) < evaluate(node->rhs);
  if(node->type == Node::Type::LEQ) return evaluate(node->lhs) <= evaluate(node->rhs);
  if(node->type == Node::Type::GT) return evaluate(node->lhs) > evaluate(node->rhs);
  if(node->type == Node::Type::GEQ) return evaluate(node->lhs) >= evaluate(node->rhs);
  if(node->type == Node::Type::RShift) return evaluate(node->lhs) >> evaluate(node->rhs);
  if(node->type == Node::Type::LShift) return evaluate(node->lhs) << evaluate(node->rhs);
  if(node->type == Node::Type::Add) return evaluate(node->lhs) + evaluate(node->rhs);
  if(node->type == Node::Type::Sub) return evaluate(node->lhs) - evaluate(node->rhs);
  if(node->type == Node::Type::Mul) return evaluate(node->lhs) * evaluate(node->rhs);
  if(node->type == Node::Type::Div) return evaluate(node->lhs) / evaluate(node->rhs);
  if(node->type == Node::Type::Mod) return evaluate(node->lhs) % evaluate(node->rhs);
  if(node->type == Node::Type::UnaryAdd) return evaluate(node->lhs) + 1;
  if(node->type == Node::Type::UnarySub) return evaluate(node->lhs) - 1;
  if(node->type == Node::Type::PostAdd) return evaluate(node->lhs) + 1;
  if(node->type == Node::Type::PostSub) return evaluate(node->lhs) - 1;
  if(node->type == Node::Type::Num) return node->val;
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
  } else if(tokens.type_is(Token::Type::Number)) {
    Node* node = new Node(tokens.at(0).val);
    tokens.pop();
    return node;
  } else if(tokens.type_is(Token::Type::Identifier)) {
    Node* node = new Node(tokens.at(0).str);
    tokens.pop();
    return node;
  }
}
