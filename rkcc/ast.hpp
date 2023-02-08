#pragma once
#include "token.hpp"
#include <string>

class Node {
public:
  enum class Type {
    Program,
    Func,
    Compound,
    Statement,
    // Program Flow
    If,
    IfElse,
    While,
    DoWhile,
    For,
    Continue,
    Break,
    Return,
    // Statement
    Assignment,  // =
    Cond,
    LogicalOr,
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
  };

  Type type;
  std::vector<Node*> childs;            // 子ノード
  Node *lhs = nullptr, *rhs = nullptr;  // 演算子の左辺・右辺
  Node* cond = nullptr;                 // cond, for, while, if の条件文
  Node* init = nullptr;                 // for の前処理
  Node* stmt = nullptr;                 // for, while の本体
  Node* iterate = nullptr;              // for の後処理
  Node* true_stmt = nullptr;            // if の本体
  Node* false_stmt = nullptr;           // else の本体
  int val;
  std::string str;
  uint16_t addr;  // 評価値のアドレス
  uint16_t reg;   // レジスタ

  // ノードのコンストラクタ
  Node(Type type, Node* cond, Node* lhs, Node* rhs);  // 三項演算子
  Node(Type type, Node* lhs, Node* rhs);              // 二項演算子
  Node(Type type, Node* lhs);                         // 単項演算子
  Node(Type type);                                    // 末端
  Node(int val);                                      // 定数値
  Node(std::string ident);                            // 識別子
};

std::string print(Node* node);

Node* program(Tokens&);
Node* func(Tokens&);
Node* compound(Tokens&);

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
Node* ident(Tokens&);
