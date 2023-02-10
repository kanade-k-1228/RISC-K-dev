#include "ast.hpp"
#include "../utils/utils.hpp"
#include <iostream>
#include <string>
#include <vector>

Node::Node(Type type, Node* cond, Node* lhs, Node* rhs)
    : type(type), lhs(lhs), rhs(rhs), cond(cond), val(0) {}

Node::Node(Type type, Node* lhs, Node* rhs)
    : type(type), lhs(lhs), rhs(rhs), cond(nullptr), val(0) {}

Node::Node(Type type, Node* lhs)
    : type(type), lhs(lhs), rhs(nullptr), cond(nullptr), val(0) {}

Node::Node(Type type)
    : type(type) {}

Node::Node(int val)
    : type(Type::Num), lhs(nullptr), rhs(nullptr), cond(nullptr), val(val) {}

Node::Node(std::string ident)
    : type(Type::Ident), lhs(nullptr), rhs(nullptr), cond(nullptr), str(ident) {}

std::string print(Node* node) {
  std::stringstream ss;
  if(node->type == Node::Type::Program) {
    for(auto n : node->childs) ss << print(n);
    return ss.str();
  }

  if(node->type == Node::Type::Type) {
    for(auto n : node->childs) ss << print(n);
    return ss.str();
  }

  if(node->type == Node::Type::Func) {
    ss << print(node->ident_type) << " " << node->str << "(";
    for(auto n : node->childs) ss << n->str << ",";
    ss << ")" << print(node->stmt);
    return ss.str();
  }

  if(node->type == Node::Type::Compound) {
    ss << "{" << std::endl;
    for(auto n : node->childs) ss << print(n) << std::endl;
    ss << "}";
    return ss.str();
  }

  if(node->type == Node::Type::If) return "if " + print(node->cond) + print(node->true_stmt);
  if(node->type == Node::Type::IfElse) return "if " + print(node->cond) + print(node->true_stmt) + "else" + print(node->false_stmt);
  if(node->type == Node::Type::While) return "while " + print(node->cond) + print(node->stmt);
  if(node->type == Node::Type::DoWhile) return "do " + print(node->stmt) + "while " + print(node->cond);
  if(node->type == Node::Type::For) return "for " + print(node->init) + print(node->cond) + print(node->iterate) + print(node->stmt);

  if(node->type == Node::Type::Continue) return "continue;";
  if(node->type == Node::Type::Break) return "break;";
  if(node->type == Node::Type::Return) return "return " + print(node->stmt) + ";";

  if(node->type == Node::Type::Statement) {
    if(node->stmt == nullptr)
      return ";";
    else
      return print(node->stmt) + ";";
  }

  if(node->type == Node::Type::Assignment) return "(" + print(node->lhs) + " = " + print(node->rhs) + ")";
  if(node->type == Node::Type::Cond) return "(" + print(node->cond) + " ? " + print(node->lhs) + " : " + print(node->rhs) + ")";
  if(node->type == Node::Type::LogicalOr) return "(" + print(node->lhs) + " || " + print(node->rhs) + ")";
  if(node->type == Node::Type::LogicalAnd) return "(" + print(node->lhs) + " && " + print(node->rhs) + ")";
  if(node->type == Node::Type::BitOr) return "(" + print(node->lhs) + " | " + print(node->rhs) + ")";
  if(node->type == Node::Type::BitXor) return "(" + print(node->lhs) + " ^ " + print(node->rhs) + ")";
  if(node->type == Node::Type::BitAnd) return "(" + print(node->lhs) + " & " + print(node->rhs) + ")";
  if(node->type == Node::Type::EQ) return "(" + print(node->lhs) + " == " + print(node->rhs) + ")";
  if(node->type == Node::Type::NEQ) return "(" + print(node->lhs) + " != " + print(node->rhs) + ")";
  if(node->type == Node::Type::LT) return "(" + print(node->lhs) + " < " + print(node->rhs) + ")";
  if(node->type == Node::Type::LEQ) return "(" + print(node->lhs) + " <= " + print(node->rhs) + ")";
  if(node->type == Node::Type::GT) return "(" + print(node->lhs) + " > " + print(node->rhs) + ")";
  if(node->type == Node::Type::GEQ) return "(" + print(node->lhs) + " >= " + print(node->rhs) + ")";
  if(node->type == Node::Type::RShift) return "(" + print(node->lhs) + " >> " + print(node->rhs) + ")";
  if(node->type == Node::Type::LShift) return "(" + print(node->lhs) + " << " + print(node->rhs) + ")";
  if(node->type == Node::Type::Add) return "(" + print(node->lhs) + " + " + print(node->rhs) + ")";
  if(node->type == Node::Type::Sub) return "(" + print(node->lhs) + " - " + print(node->rhs) + ")";
  if(node->type == Node::Type::Mul) return "(" + print(node->lhs) + " * " + print(node->rhs) + ")";
  if(node->type == Node::Type::Div) return "(" + print(node->lhs) + " / " + print(node->rhs) + ")";
  if(node->type == Node::Type::Mod) return "(" + print(node->lhs) + " % " + print(node->rhs) + ")";
  if(node->type == Node::Type::UnaryAdd) return "(++ " + print(node->lhs) + ")";
  if(node->type == Node::Type::UnarySub) return "(-- " + print(node->lhs) + ")";
  if(node->type == Node::Type::PostAdd) return "(" + print(node->lhs) + " ++)";
  if(node->type == Node::Type::PostSub) return "(" + print(node->lhs) + " --)";
  if(node->type == Node::Type::Num) return std::to_string(node->val);
  if(node->type == Node::Type::Ident) return node->str;
  return "NO PRINT FORMAT";
}

Node* program(Tokens& tokens) {
  Node* root = new Node(Node::Type::Program);
  while(!tokens.empty()) root->childs.push_back(func(tokens));
  return root;
}

Node* type(Tokens& tokens) {
  Node* node = new Node(Node::Type::Type);
  node->childs.push_back(new Node(tokens.pop().str));
  return node;
}

Node* func(Tokens& tokens) {
  Node* node = new Node(Node::Type::Func);
  node->ident_type = type(tokens);
  node->str = tokens.pop().str;
  if(!tokens.consume("(")) error("Expect (");
  for(; !tokens.consume(")"); tokens.pop()) {
    node->childs.push_back(ident(tokens));
    if(tokens.consume(")")) break;
  }
  node->stmt = compound(tokens);
  return node;
}

Node* compound(Tokens& tokens) {
  Node* node = new Node(Node::Type::Compound);
  if(!tokens.consume("{")) error("Expect {");
  while(!tokens.consume("}")) node->childs.push_back(stmt(tokens));
  return node;
}

Node* stmt(Tokens& tokens) {
  if(tokens.consume(";")) {
    return new Node(Node::Type::Statement);
  } else if(tokens.head_is("{")) {
    return compound(tokens);
  } else if(tokens.consume("if")) {
    Node* if_node = new Node(Node::Type::If);
    tokens.consume("(");
    if_node->cond = expr(tokens);
    tokens.consume(")");
    if_node->true_stmt = stmt(tokens);
    if(tokens.consume("else")) {
      if_node->type = Node::Type::IfElse;
      if_node->false_stmt = stmt(tokens);
    }
    return if_node;
  } else if(tokens.consume("while")) {
    Node* while_node = new Node(Node::Type::While);
    tokens.consume("(");
    while_node->cond = expr(tokens);
    tokens.consume(")");
    while_node->stmt = stmt(tokens);
    return while_node;
  } else if(tokens.consume("do")) {
    Node* do_while_node = new Node(Node::Type::DoWhile);
    do_while_node->stmt = stmt(tokens);
    tokens.consume("while");
    tokens.consume("(");
    do_while_node->cond = expr(tokens);
    tokens.consume(")");
    tokens.consume(";");
    return do_while_node;
  } else if(tokens.consume("for")) {
    Node* for_node = new Node(Node::Type::For);
    tokens.consume("(");
    for_node->init = expr(tokens);
    tokens.consume(";");
    for_node->cond = expr(tokens);
    tokens.consume(";");
    for_node->iterate = expr(tokens);
    tokens.consume(")");
    for_node->stmt = stmt(tokens);
    return for_node;
  } else if(tokens.consume("continue")) {
    Node* continue_node = new Node(Node::Type::Continue);
    tokens.consume(";");
    return continue_node;
  } else if(tokens.consume("break")) {
    Node* break_node = new Node(Node::Type::Break);
    tokens.consume(";");
    return break_node;
  } else if(tokens.consume("return")) {
    Node* return_node = new Node(Node::Type::Return);
    return_node->stmt = expr(tokens);
    tokens.consume(";");
    return return_node;
  } else {
    Node* node = new Node(Node::Type::Statement);
    node->stmt = expr(tokens);
    tokens.consume(";");
    return node;
  }
}

Node* expr(Tokens& tokens) {
  Node* node = assign(tokens);
  return node;
}

Node* assign(Tokens& tokens) {
  Node* node = cond(tokens);
  if(tokens.consume("=")) {
    node = new Node(Node::Type::Assignment, node, assign(tokens));
  }
  return node;
}

Node* cond(Tokens& tokens) {
  Node* node = logical_or(tokens);
  if(tokens.consume("?")) {
    Node* true_expr = expr(tokens);
    tokens.consume(":");
    Node* false_expr = cond(tokens);
    node = new Node(Node::Type::Cond, node, true_expr, false_expr);
  }
  return node;
}

Node* logical_or(Tokens& tokens) {
  Node* node = logical_and(tokens);
  while(tokens.consume("||"))
    node = new Node(Node::Type::LogicalOr, node, logical_and(tokens));
  return node;
}

Node* logical_and(Tokens& tokens) {
  Node* node = bit_or(tokens);
  while(tokens.consume("&&"))
    node = new Node(Node::Type::LogicalAnd, node, bit_or(tokens));
  return node;
}

Node* bit_or(Tokens& tokens) {
  Node* node = bit_xor(tokens);
  while(tokens.consume("|"))
    node = new Node(Node::Type::BitOr, node, bit_xor(tokens));
  return node;
}

Node* bit_xor(Tokens& tokens) {
  Node* node = bit_and(tokens);
  while(tokens.consume("^"))
    node = new Node(Node::Type::BitXor, node, bit_and(tokens));
  return node;
}

Node* bit_and(Tokens& tokens) {
  Node* node = equality(tokens);
  while(tokens.consume("&"))
    node = new Node(Node::Type::BitAnd, node, equality(tokens));
  return node;
}

Node* equality(Tokens& tokens) {
  Node* node = relational(tokens);
  if(tokens.consume("=="))
    node = new Node(Node::Type::EQ, node, relational(tokens));
  else if(tokens.consume("!="))
    node = new Node(Node::Type::NEQ, node, relational(tokens));
  return node;
}

Node* relational(Tokens& tokens) {
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
    tokens.consume(")");
    return node;
  } else if(tokens.head().type_is(Token::Type::Number)) {
    Node* node = new Node(tokens.head().val);
    tokens.pop();
    return node;
  } else if(tokens.head().type_is(Token::Type::Identifier)) {
    return ident(tokens);
    Node* node = new Node(tokens.head().str);
    tokens.pop();
    return node;
  } else {
    error("Expected Primitive: " + tokens.head().str);
    return nullptr;
  }
}

Node* ident(Tokens& tokens) { return new Node(tokens.pop().str); }
