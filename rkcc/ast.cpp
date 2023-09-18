#include "ast.hpp"
#include "utils.hpp"
#include <iostream>
#include <string>
#include <vector>

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
      Node* node_child = type(tokens);
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
    return new Node(Node::Type::TypePrim, {new Node(tokens.pop().str)});
  }
  return nullptr;
}

Node* program(Tokens& tokens) {
  Node* program_root = new Node(Node::Type::Program);
  while(!tokens.empty()) {
    if(tokens.consume("func")) {
      Node* func_name = ident(tokens);
      tokens.consume(":");
      Node* func_type = type(tokens);
      Node* func_body = compound(tokens);
      program_root->add_child(new Node(Node::Type::FuncDef, {func_name, func_type, func_body}));
    } else if(tokens.consume("var")) {
      Node* gvar_name = ident(tokens);
      tokens.consume(":");
      Node* gvar_type = type(tokens);
      tokens.consume(";");
      program_root->add_child(new Node(Node::Type::GVarDef, {gvar_name, gvar_type}));
    } else if(tokens.consume("type")) {
      Node* type_name = ident(tokens);
      tokens.consume(":");
      Node* type_type = type(tokens);
      tokens.consume(";");
      program_root->add_child(new Node(Node::Type::TypeDef, {type_name, type_type}));
    }
  }
  return program_root;
}

Node* compound(Tokens& tokens) {
  Node* node = new Node(Node::Type::Compound);
  tokens.consume("{");
  while(!tokens.consume("}")) node->add_child(stmt(tokens));
  return node;
}

Node* stmt(Tokens& tokens) {
  if(tokens.consume(";")) {
    return new Node(Node::Type::VoidStmt);
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
    Node* lvar_name = ident(tokens);
    tokens.consume(":");
    Node* lvar_type = type(tokens);
    tokens.consume(";");
    return new Node(Node::Type::LVarDef, {lvar_name, lvar_type});
  } else {
    Node* body = expr(tokens);
    if(tokens.consume(";")) {  // 式文
      return new Node(Node::Type::ExprStmt, {body});
    } else if(tokens.consume("=")) {  // 代入文
      Node* rhs = expr(tokens);
      tokens.consume(";");
      return new Node(Node::Type::Assign, {body, rhs});
    }
  }
  throw new std::string("Cannot read as stmt" + tokens.head().str);
  return nullptr;
}

Node* expr(Tokens& tokens) { return cond(tokens); }

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
  Node* node = post(tokens);
  for(;;) {
    if(tokens.consume("**"))
      node = new Node(Node::Type::Mul, {node, post(tokens)});
    else if(tokens.consume("//"))
      node = new Node(Node::Type::Div, {node, post(tokens)});
    else if(tokens.consume("%%"))
      node = new Node(Node::Type::Mod, {node, post(tokens)});
    else
      return node;
  }
}

Node* post(Tokens& tokens) {
  Node* node = prim(tokens);
  for(;;) {
    if(tokens.consume(":")) {
      Node* cast_type = type(tokens);
      node = new Node(Node::Type::Cast, {node, cast_type});
    } else if(tokens.consume("*")) {
      node = new Node(Node::Type::Ref, {node});
    } else if(tokens.consume("@")) {
      node = new Node(Node::Type::Addr, {node});
    } else if(tokens.consume("[")) {
      Node* array_sufix = expr(tokens);
      tokens.consume("]");
      node = new Node(Node::Type::Array, {node, array_sufix});
    } else if(tokens.consume(".")) {
      Node* member_name = ident(tokens);
      node = new Node(Node::Type::Member, {node, member_name});
    } else if(tokens.consume("(")) {
      node = new Node(Node::Type::FuncCall, {node});
      while(!tokens.consume(")")) node->add_child(ident(tokens));
      return node;
    } else {
      return node;
    }
  }
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
    throw new std::string("Expected Primitive: " + tokens.head().str);
    return nullptr;
  }
}

Node* num(Tokens& tokens) { return new Node(tokens.pop().val); }
Node* ident(Tokens& tokens) { return new Node(tokens.pop().str); }
