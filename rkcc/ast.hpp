#pragma once
#include "token.hpp"
#include <string>

class Node {
public:
  enum class Type {
    Cond,
    LogicalOr,
    LogicalXor,
    LogicalAnd,
    BitOr,
    BitXor,
    BitAnd,
    EQ,   // ==
    NEQ,  // !=
    LT,   // <
    LEQ,  // <=
    GT,   // >
    GEQ,  // >=
    RShift,
    LShift,
    Add,
    Sub,
    Mul,
    Div,
    Mod,
    UnaryAdd,
    UnarySub,
    PostAdd,
    PostSub,
    Num
  };
  // 三項演算子のコンストラクタ
  Node(Type type, Node* cond, Node* lhs, Node* rhs);
  // 二項演算子のコンストラクタ
  Node(Type type, Node* lhs, Node* rhs);
  // 単項演算子のコンストラクタ
  Node(Type type, Node* lhs);
  // 数値のコンストラクタ
  Node(int val);
  Type type;
  Node *lhs, *rhs, *cond;
  int val;
  std::string print();
  int evaluate();
};

Node* expr(Tokens&);
Node* cond(Tokens&);
Node* l_or(Tokens&);
Node* l_xor(Tokens&);
Node* l_and(Tokens&);
Node* b_or(Tokens&);
Node* b_xor(Tokens&);
Node* b_and(Tokens&);
Node* equal(Tokens&);
Node* relate(Tokens&);
Node* shift(Tokens&);
Node* add(Tokens&);
Node* mul(Tokens&);
Node* unary(Tokens&);
Node* post(Tokens&);
Node* prim(Tokens&);
