#pragma once
#include "token.hpp"
#include <string>

class Node {
public:
  enum class Type {
    Add,
    Sub,
    Mul,
    Div,
    Num
  };
  // 二項演算子のコンストラクタ
  Node(Type type, Node* lhs, Node* rhs);
  // 数値のコンストラクタ
  Node(int val);
  Type type;
  Node *lhs, *rhs;
  int val;
  std::string print();
  int reduct();
};

Node* expr(Tokens&);
Node* mul(Tokens&);
Node* primary(Tokens&);

Node* constant_expr(Tokens&);
Node* conditional_expr(Tokens&);
Node* logical_or_expr(Tokens&);
Node* logical_or_expr(Tokens&);
Node* logical_and_expr(Tokens&);
Node* inclusive_or_expr(Tokens&);
Node* exclusive_or_expr(Tokens&);
Node* and_expr(Tokens&);
Node* equality_expr(Tokens&);
Node* relational_expr(Tokens&);
Node* shift_expr(Tokens&);
Node* additive_expr(Tokens&);
Node* multiplicative_expr(Tokens&);
Node* cast_expr(Tokens&);
Node* unary_expr(Tokens&);
Node* postfix_expr(Tokens&);
Node* primary_expr(Tokens&);
Node* constant(Tokens&);

Node* exprr(Tokens&);
Node* assignment_expr(Tokens&);
Node* assignment_operator(Tokens&);
