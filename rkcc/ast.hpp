#pragma once
#include "token.hpp"
#include <string>

class Node {
public:
  enum class Type {
    Program,
    // Type
    Type,
    TypeFunc,
    TypeStruct,
    TypeArray,
    TypePointer,
    TypePrim,
    // Func
    Func,
    Compound,
    VarDef,
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
    Expr,
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
  std::vector<Node*> childs;  // 子ノード
  int val;                    // 数値
  std::string str;            // 文字列

  // ノードのコンストラクタ
  Node(Type type) : type(type) {}
  Node(Type type, std::vector<Node*> childs) : type(type), childs(childs) {}
  Node(int val) : type(Type::Num), val(val) {}                // 定数値
  Node(std::string ident) : type(Type::Ident), str(ident) {}  // 識別子

  bool type_is(Type t) { return type == t; }

  void add_child(Node* node) { childs.push_back(node); }

  Node* type_base() { return childs.at(0); }
  int array_len() { return childs.at(1)->val; }
  std::vector<Node*> type_members() { return childs; }

  Node* func_name() { return childs.at(0); }
  Node* func_type() { return childs.at(1); }
  Node* func_stmt() { return childs.at(2); }

  Node* ctrl_cond() { return childs.at(0); }     // 条件文
  Node* ctrl_body() { return childs.at(1); }     // 本体
  Node* ctrl_true() { return childs.at(1); }     // 真の処理
  Node* ctrl_false() { return childs.at(2); }    // 偽の処理
  Node* ctrl_init() { return childs.at(0); }     // 初期化処理
  Node* ctrl_iterate() { return childs.at(3); }  // 次に進む処理

  // 三項演算
  Node* tri_cond() { return childs.at(0); }   // 条件文
  Node* tri_true() { return childs.at(1); }   // 真
  Node* tri_false() { return childs.at(2); }  // 偽

  // 二項演算
  Node* lhs() { return childs.at(0); };  // 左側
  Node* rhs() { return childs.at(1); };  // 右側

  // 単項演算
  Node* child() { return childs.at(0); }
};

std::string print(Node* node);

Node* program(Tokens&);
Node* type(Tokens&);
Node* type_func(Tokens&);
Node* type_struct(Tokens&);

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
Node* num(Tokens&);
Node* ident(Tokens&);
