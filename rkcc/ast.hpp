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
    Num,
    Ident,
    Statement,
    CompoundStmt,
    If,
    IfElse,
    While,
    DoWhile,
    For,
    Program,
    Assignment,  // =
  };
  Type type;
  Node *lhs = nullptr, *rhs = nullptr;  // 演算子の左辺・右辺
  Node* cond = nullptr;                 // cond, for, while, if の条件文
  Node* init = nullptr;                 // for の前処理
  Node* stmt = nullptr;                 // for, while の本体
  Node* iterate = nullptr;              // for の後処理
  Node* true_stmt = nullptr;            // if の本体
  Node* false_stmt = nullptr;           // else の本体
  int val;
  std::string str;
  // 三項演算子
  Node(Type type, Node* cond, Node* lhs, Node* rhs);
  // 二項演算子
  Node(Type type, Node* lhs, Node* rhs);
  // 単項演算子
  Node(Type type, Node* lhs);
  // 末端
  Node(Type type);
  // 数値
  Node(int val);
  // 識別子
  Node(std::string ident);
};

std::string print(Node* node);
int evaluate(Node* node);

Node* program(Tokens&);
Node* stmt(Tokens&);
Node* expr(Tokens&);
Node* assign(Tokens&);
Node* cond(Tokens&);
Node* logical_or(Tokens&);
Node* logical_xor(Tokens&);
Node* logical_and(Tokens&);
Node* bit_or(Tokens&);
Node* bit_xor(Tokens&);
Node* bit_and(Tokens&);
Node* equality(Tokens&);
Node* relational(Tokens&);
Node* shift(Tokens&);
Node* add(Tokens&);
Node* mul(Tokens&);
Node* unary(Tokens&);
Node* post(Tokens&);
Node* prim(Tokens&);
