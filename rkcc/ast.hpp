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
