#include "ast.hpp"
#include "../utils/utils.hpp"
#include <iostream>
#include <string>
#include <vector>

std::string print(Node* node) {
  std::stringstream ss;
  if(node->type_is(Node::Type::Program)) {
    for(auto n : node->childs) ss << print(n);
    return ss.str();
  }

  if(node->type_is(Node::Type::Type)) {
    for(auto n : node->childs) ss << print(n);
    return ss.str();
  }

  if(node->type_is(Node::Type::TypeFunc)) {
    int arg_n = node->childs.size() / 2;
    ss << "( ";
    for(int i = 0; i < arg_n; ++i)
      ss << print(node->childs.at(i * 2)) << " : "
         << print(node->childs.at(i * 2 + 1))
         << (i == arg_n - 1 ? " " : ", ");
    ss << ") => " << print(node->childs.at(node->childs.size() - 1));
    return ss.str();
  }

  if(node->type_is(Node::Type::TypeStruct)) {
    int n_member = node->childs.size() / 2;
    ss << "{ ";
    for(int i = 0; i < n_member; ++i)
      ss << print(node->childs.at(i * 2)) << " : "
         << print(node->childs.at(i * 2 + 1))
         << (i == n_member - 1 ? " " : ", ");
    ss << "}";
    return ss.str();
  }

  if(node->type_is(Node::Type::TypeArray)) {
    ss << "[" << print(node->childs.at(1)) << "]" << print(node->childs.at(0));
    return ss.str();
  }

  if(node->type_is(Node::Type::TypePointer)) {
    ss << "*" << print(node->childs.at(0));
    return ss.str();
  }

  if(node->type_is(Node::Type::Func)) {
    ss << "func " << print(node->func_name())
       << " : " << print(node->func_type())
       << " " << print(node->func_stmt());
    return ss.str();
  }

  if(node->type_is(Node::Type::Compound)) {
    ss << "{" << std::endl;
    for(auto n : node->childs) ss << print(n) << std::endl;
    ss << "}";
    return ss.str();
  }

  if(node->type_is(Node::Type::VarDef)) {
    ss << "var " << print(node->childs.at(0))
       << " : " << print(node->childs.at(1));
    if(node->childs.size() > 2)
      ss << " = " << print(node->childs.at(2));
    ss << ";";
    return ss.str();
  }

  if(node->type == Node::Type::If) return "if " + print(node->ctrl_cond()) + print(node->ctrl_true());
  if(node->type == Node::Type::IfElse) return "if " + print(node->ctrl_cond()) + print(node->ctrl_true()) + "else" + print(node->ctrl_false());
  if(node->type == Node::Type::While) return "while " + print(node->ctrl_cond()) + print(node->ctrl_body());
  if(node->type == Node::Type::DoWhile) return "do " + print(node->ctrl_body()) + "while " + print(node->ctrl_cond());
  if(node->type == Node::Type::For) return "for " + print(node->ctrl_init()) + print(node->ctrl_cond()) + print(node->ctrl_iterate()) + print(node->ctrl_body());

  if(node->type == Node::Type::Continue) return "continue;";
  if(node->type == Node::Type::Break) return "break;";
  if(node->type == Node::Type::Return) return "return " + print(node->child()) + ";";

  if(node->type == Node::Type::Expr) {
    if(node->childs.size() == 0)
      return ";";
    else
      return print(node->child()) + ";";
  }

  if(node->type == Node::Type::Assignment) return "(" + print(node->lhs()) + " = " + print(node->rhs()) + ")";
  if(node->type == Node::Type::Cond) return "(" + print(node->tri_true()) + " ? " + print(node->tri_true()) + " : " + print(node->tri_false()) + ")";
  if(node->type == Node::Type::LogicalOr) return "(" + print(node->lhs()) + " || " + print(node->rhs()) + ")";
  if(node->type == Node::Type::LogicalAnd) return "(" + print(node->lhs()) + " && " + print(node->rhs()) + ")";
  if(node->type == Node::Type::BitOr) return "(" + print(node->lhs()) + " | " + print(node->rhs()) + ")";
  if(node->type == Node::Type::BitXor) return "(" + print(node->lhs()) + " ^ " + print(node->rhs()) + ")";
  if(node->type == Node::Type::BitAnd) return "(" + print(node->lhs()) + " & " + print(node->rhs()) + ")";
  if(node->type == Node::Type::EQ) return "(" + print(node->lhs()) + " == " + print(node->rhs()) + ")";
  if(node->type == Node::Type::NEQ) return "(" + print(node->lhs()) + " != " + print(node->rhs()) + ")";
  if(node->type == Node::Type::LT) return "(" + print(node->lhs()) + " < " + print(node->rhs()) + ")";
  if(node->type == Node::Type::LEQ) return "(" + print(node->lhs()) + " <= " + print(node->rhs()) + ")";
  if(node->type == Node::Type::GT) return "(" + print(node->lhs()) + " > " + print(node->rhs()) + ")";
  if(node->type == Node::Type::GEQ) return "(" + print(node->lhs()) + " >= " + print(node->rhs()) + ")";
  if(node->type == Node::Type::RShift) return "(" + print(node->lhs()) + " >> " + print(node->rhs()) + ")";
  if(node->type == Node::Type::LShift) return "(" + print(node->lhs()) + " << " + print(node->rhs()) + ")";
  if(node->type == Node::Type::Add) return "(" + print(node->lhs()) + " + " + print(node->rhs()) + ")";
  if(node->type == Node::Type::Sub) return "(" + print(node->lhs()) + " - " + print(node->rhs()) + ")";
  if(node->type == Node::Type::Mul) return "(" + print(node->lhs()) + " * " + print(node->rhs()) + ")";
  if(node->type == Node::Type::Div) return "(" + print(node->lhs()) + " / " + print(node->rhs()) + ")";
  if(node->type == Node::Type::Mod) return "(" + print(node->lhs()) + " % " + print(node->rhs()) + ")";
  if(node->type == Node::Type::UnaryAdd) return "(++ " + print(node->lhs()) + ")";
  if(node->type == Node::Type::UnarySub) return "(-- " + print(node->lhs()) + ")";
  if(node->type == Node::Type::PostAdd) return "(" + print(node->lhs()) + " ++)";
  if(node->type == Node::Type::PostSub) return "(" + print(node->lhs()) + " --)";
  if(node->type == Node::Type::Num) return std::to_string(node->val);
  if(node->type == Node::Type::Ident) return node->str;
  return "NO PRINT FORMAT";
}

Node* program(Tokens& tokens) {
  Node* root = new Node(Node::Type::Program);
  while(!tokens.empty()) root->add_child(func(tokens));
  return root;
}

Node* type(Tokens& tokens) {
  // 構造体
  if(tokens.consume("{")) {
    Node* node_struct = new Node(Node::Type::TypeStruct);
    while(!tokens.consume("}")) {
      node_struct->add_child(ident(tokens));  // Member Name
      tokens.consume(":");
      node_struct->add_child(type(tokens));  // Member Type
      if(tokens.consume(",")) continue;
      if(tokens.consume("}")) break;
    }
    return node_struct;
  }
  if(tokens.consume("(")) {
    int close = tokens.pair_bracket(1, "(", ")");
    if(tokens.head(close + 1).str_is("=>")) {  // 関数
      Node* node_func = new Node(Node::Type::TypeFunc);
      while(!tokens.consume(")")) {
        node_func->add_child(ident(tokens));  // Arg Name
        tokens.consume(":");
        node_func->add_child(type(tokens));  // Arg Type
        if(tokens.consume(",")) continue;
        if(tokens.consume(")")) break;
      }
      tokens.consume("=>");
      node_func->add_child(type(tokens));  // Return Type
      return node_func;
    } else {  // 括弧
      Node* node_child = new Node(Node::Type::Type, {type(tokens)});
      tokens.consume(")");
      return node_child;
    }
  }
  if(tokens.consume("[")) {  // 配列
    Node* array_len = expr(tokens);
    tokens.consume("]");
    Node* base_type = type(tokens);
    return new Node(Node::Type::TypeArray, {base_type, array_len});
  }
  if(tokens.consume("*")) {  // ポインタ
    return new Node(Node::Type::TypePointer, {type(tokens)});
  }
  // ベース型
  if(tokens.head().type_is(Token::Type::Identifier)) {
    return new Node(tokens.pop().str);
  }
  return nullptr;
}

Node* func(Tokens& tokens) {
  Node* node = new Node(Node::Type::Func);
  tokens.consume("func");
  node->add_child(ident(tokens));  // Function Name
  tokens.consume(":");
  node->add_child(type(tokens));      // Function Type
  node->add_child(compound(tokens));  // Function Body
  return node;
}

Node* compound(Tokens& tokens) {
  Node* node = new Node(Node::Type::Compound);
  tokens.consume("{");
  while(!tokens.consume("}")) node->add_child(stmt(tokens));
  return node;
}

Node* stmt(Tokens& tokens) {
  if(tokens.consume(";")) {
    return new Node(Node::Type::Expr);
  } else if(tokens.head().str_is("{")) {
    return compound(tokens);
  } else if(tokens.consume("if")) {
    tokens.consume("(");
    Node* cond = expr(tokens);
    tokens.consume(")");
    Node* true_stmt = stmt(tokens);
    if(tokens.consume("else")) {
      return new Node(Node::Type::IfElse, {cond, true_stmt, stmt(tokens)});
    } else {
      return new Node(Node::Type::If, {cond, true_stmt});
    }
  } else if(tokens.consume("while")) {
    tokens.consume("(");
    Node* cond = expr(tokens);
    tokens.consume(")");
    Node* body = stmt(tokens);
    return new Node(Node::Type::While, {cond, body});
  } else if(tokens.consume("do")) {
    Node* body = stmt(tokens);
    tokens.consume("while");
    tokens.consume("(");
    Node* cond = expr(tokens);
    tokens.consume(")");
    tokens.consume(";");
    return new Node(Node::Type::DoWhile, {cond, body});
  } else if(tokens.consume("for")) {
    tokens.consume("(");
    Node* for_init = expr(tokens);
    tokens.consume(";");
    Node* for_cond = expr(tokens);
    tokens.consume(";");
    Node* for_iterate = expr(tokens);
    tokens.consume(")");
    Node* for_body = stmt(tokens);
    return new Node(Node::Type::For, {for_cond, for_body, for_init, for_iterate});
  } else if(tokens.consume("continue")) {
    tokens.consume(";");
    return new Node(Node::Type::Continue);
  } else if(tokens.consume("break")) {
    tokens.consume(";");
    return new Node(Node::Type::Break);
  } else if(tokens.consume("return")) {
    Node* ret = expr(tokens);
    tokens.consume(";");
    return new Node(Node::Type::Return, {ret});
  } else if(tokens.consume("var")) {
    Node* var = new Node(Node::Type::VarDef);
    var->add_child(ident(tokens));
    tokens.consume(":");
    var->add_child(type(tokens));
    if(tokens.consume("=")) var->add_child(expr(tokens));
    tokens.consume(";");
    return var;
  } else {
    Node* body = expr(tokens);
    tokens.consume(";");
    return new Node(Node::Type::Expr, {body});
  }
}

Node* expr(Tokens& tokens) {
  return assign(tokens);
}

Node* assign(Tokens& tokens) {
  Node* node = cond(tokens);
  if(tokens.consume("="))
    node = new Node(Node::Type::Assignment, {node, assign(tokens)});
  return node;
}

Node* cond(Tokens& tokens) {
  Node* node = logical_or(tokens);
  if(tokens.consume("?")) {
    Node* true_expr = expr(tokens);
    tokens.consume(":");
    Node* false_expr = cond(tokens);
    node = new Node(Node::Type::Cond, {node, true_expr, false_expr});
  }
  return node;
}

Node* logical_or(Tokens& tokens) {
  Node* node = logical_and(tokens);
  while(tokens.consume("||"))
    node = new Node(Node::Type::LogicalOr, {node, logical_and(tokens)});
  return node;
}

Node* logical_and(Tokens& tokens) {
  Node* node = bit_or(tokens);
  while(tokens.consume("&&"))
    node = new Node(Node::Type::LogicalAnd, {node, bit_or(tokens)});
  return node;
}

Node* bit_or(Tokens& tokens) {
  Node* node = bit_xor(tokens);
  while(tokens.consume("|"))
    node = new Node(Node::Type::BitOr, {node, bit_xor(tokens)});
  return node;
}

Node* bit_xor(Tokens& tokens) {
  Node* node = bit_and(tokens);
  while(tokens.consume("^"))
    node = new Node(Node::Type::BitXor, {node, bit_and(tokens)});
  return node;
}

Node* bit_and(Tokens& tokens) {
  Node* node = equality(tokens);
  while(tokens.consume("&"))
    node = new Node(Node::Type::BitAnd, {node, equality(tokens)});
  return node;
}

Node* equality(Tokens& tokens) {
  Node* node = relational(tokens);
  if(tokens.consume("=="))
    node = new Node(Node::Type::EQ, {node, relational(tokens)});
  else if(tokens.consume("!="))
    node = new Node(Node::Type::NEQ, {node, relational(tokens)});
  return node;
}

Node* relational(Tokens& tokens) {
  Node* node = shift(tokens);
  if(tokens.consume("<"))
    node = new Node(Node::Type::LT, {node, shift(tokens)});
  else if(tokens.consume("<="))
    node = new Node(Node::Type::LEQ, {node, shift(tokens)});
  else if(tokens.consume(">"))
    node = new Node(Node::Type::GT, {node, shift(tokens)});
  else if(tokens.consume(">="))
    node = new Node(Node::Type::GEQ, {node, shift(tokens)});
  return node;
}

Node* shift(Tokens& tokens) {
  Node* node = add(tokens);
  for(;;) {
    if(tokens.consume("<<"))
      node = new Node(Node::Type::LShift, {node, add(tokens)});
    else if(tokens.consume(">>"))
      node = new Node(Node::Type::RShift, {node, add(tokens)});
    else
      return node;
  }
}

Node* add(Tokens& tokens) {
  Node* node = mul(tokens);
  for(;;) {
    if(tokens.consume("+"))
      node = new Node(Node::Type::Add, {node, mul(tokens)});
    else if(tokens.consume("-"))
      node
          = new Node(Node::Type::Sub, {node, mul(tokens)});
    else
      return node;
  }
}

Node* mul(Tokens& tokens) {
  Node* node = unary(tokens);
  for(;;) {
    if(tokens.consume("*"))
      node = new Node(Node::Type::Mul, {node, unary(tokens)});
    else if(tokens.consume("/"))
      node = new Node(Node::Type::Div, {node, unary(tokens)});
    else if(tokens.consume("%"))
      node = new Node(Node::Type::Mod, {node, unary(tokens)});
    else
      return node;
  }
}

Node* unary(Tokens& tokens) {
  if(tokens.consume("++"))
    return new Node(Node::Type::UnaryAdd, {post(tokens)});
  else if(tokens.consume("--"))
    return new Node(Node::Type::UnarySub, {post(tokens)});
  else
    return post(tokens);
}

Node* post(Tokens& tokens) {
  Node* node = prim(tokens);
  if(tokens.consume("++"))
    return new Node(Node::Type::PostAdd, {node});
  else if(tokens.consume("--"))
    return new Node(Node::Type::PostSub, {node});
  else
    return node;
}

Node* prim(Tokens& tokens) {
  if(tokens.consume("(")) {
    Node* node = expr(tokens);
    tokens.consume(")");
    return node;
  } else if(tokens.head().type_is(Token::Type::Number)) {
    return num(tokens);
  } else if(tokens.head().type_is(Token::Type::Identifier)) {
    return ident(tokens);
  } else {
    error("Expected Primitive: " + tokens.head().str);
    return nullptr;
  }
}

Node* num(Tokens& tokens) { return new Node(tokens.pop().val); }
Node* ident(Tokens& tokens) { return new Node(tokens.pop().str); }

int type_size(Node* node) {
  switch(node->type) {
  case Node::Type::TypePrim:  // 整数型 : 1
    return 1;
  case Node::Type::TypePointer:  // ポインタ : 1
    return 1;
  case Node::Type::TypeArray:  // 配列型 : ベース型のN倍
    return type_size(node->type_base()) * node->array_len();
  case Node::Type::TypeStruct:  // 構造体 : メンバの型の合計
    int ret;
    for(auto t : node->type_members())
      ret += type_size(t);
    return ret;
  case Node::Type::TypeFunc:  // 関数型 : 評価不可
    return -1;
  default:
    error("This node is not type: " + print(node));
    return 0;
  }
}
