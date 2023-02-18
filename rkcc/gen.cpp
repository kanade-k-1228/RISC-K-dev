#include "gen.hpp"
#include "../utils/utils.hpp"
#include <iostream>

void CodeGen::gen_expr(Node* node) {
  if(node->type == Node::Type::Program) {
    for(auto n : node->childs) gen_expr(n);
    return;
  }
  if(node->type == Node::Type::Assign) {
    // 代入する値を計算する
    gen_expr(node->lhs());
    // 値は一時レジスタ t0 に入っているので，左辺に代入する
    // 代入式の左辺はレジスタまたはメモリアドレスである必要がある
    if(node->lhs()->type == Node::Type::Ident) {
      code.store("t0", "zero", node->lhs()->str);
    }
    return;
  }

  // 三項演算
  if(node->type == Node::Type::Cond) {
    std::string cond = "cond_id";
    // 条件式を評価
    gen_expr(node->tri_cond());
    // 条件値はt0に入っている
    code.breq("t0", "zero", cond + "_false");
    // True
    gen_expr(node->tri_true());
    code.jump("zero", "zero", cond + "_end");
    // False
    code.label(cond + "_false");
    gen_expr(node->tri_true());
    // End
    code.label(cond + "_end");
    return;
  }

  // 二項演算
  if(node->type == Node::Type::LogicalOr
     || node->type == Node::Type::LogicalAnd
     || node->type == Node::Type::BitOr
     || node->type == Node::Type::BitXor
     || node->type == Node::Type::BitAnd
     || node->type == Node::Type::EQ
     || node->type == Node::Type::NEQ
     || node->type == Node::Type::LT
     || node->type == Node::Type::LEQ
     || node->type == Node::Type::GT
     || node->type == Node::Type::GEQ
     || node->type == Node::Type::RShift
     || node->type == Node::Type::LShift
     || node->type == Node::Type::Add
     || node->type == Node::Type::Sub
     || node->type == Node::Type::Mul
     || node->type == Node::Type::Div
     || node->type == Node::Type::Mod) {
    gen_expr(node->lhs());  // 左辺値を評価し
    code.push("t0");        // スタックに退避
    gen_expr(node->rhs());  // 右辺値を評価
    code.pop("t1");         // 左辺値を戻す

    if(node->type == Node::Type::LogicalOr) code.comment("[t0 = (t0 || t1)]");
    if(node->type == Node::Type::LogicalAnd) code.comment("[t0 = (t0 && t1)]");

    if(node->type == Node::Type::BitOr) code.comment("[t0 = (t0 | t1)]");
    if(node->type == Node::Type::BitXor) code.comment("[t0 = (t0 ^ t1)]");
    if(node->type == Node::Type::BitAnd) code.comment("[t0 = (t0 & t1)]");

    if(node->type == Node::Type::EQ) code.comment("[t0 = (t0 == t1)]");
    if(node->type == Node::Type::NEQ) code.comment("[t0 = (t0 != t1)]");
    if(node->type == Node::Type::LT) code.comment("[t0 = (t0 < t1)]");
    if(node->type == Node::Type::LEQ) code.comment("[t0 = (t0 <= t1)]");
    if(node->type == Node::Type::GT) code.comment("[t0 = (t0 > t1)]");
    if(node->type == Node::Type::GEQ) code.comment("[t0 = (t0 >= t1)]");

    if(node->type == Node::Type::RShift) code.comment("[t0 = (t0 >> t1)]");
    if(node->type == Node::Type::LShift) code.comment("[t0 = (t0 << t1)]");

    if(node->type == Node::Type::Add) code.add("t0", "t0", "t1");
    if(node->type == Node::Type::Sub) code.sub("t0", "t0", "t1");

    if(node->type == Node::Type::Mul) code.comment("[t0 = (t0 * t1)]");
    if(node->type == Node::Type::Div) code.comment("[t0 = (t0 / t1)]");
    if(node->type == Node::Type::Mod) code.comment("[t0 = (t0 % t1)]");

    return;
  }

  // if(node->type == Node::Type::PostAdd) return "( " + print(node->lhs) + " ++ )";
  // if(node->type == Node::Type::PostSub) return "( " + print(node->lhs) + " -- )";

  if(node->type == Node::Type::Num) {
    // 定数値
    code.loadi("t0", node->val);
    return;
  }

  if(node->type == Node::Type::Ident) {
    // 変数の入ってるアドレスから値を取り出す
    // グローバル変数の場合
    code.load("t0", "zero", node->str);
    return;
    // ローカル変数の場合
    std::string addr = "XXX";
    code.load("t0", "fp", node->str);
    return;
  }

  // if(node->type == Node::Type::CompoundStmt) return print(node->lhs) + "\n" + print(node->rhs);

  if(node->type == Node::Type::If) {
    std::string if_id = "if_id";
    gen_expr(node->ctrl_cond());              // 条件式を評価
    code.breq("t0", "zero", if_id + "_end");  // 分岐
    gen_expr(node->ctrl_true());              // true の処理
    code.label(if_id + "_end");               // if の最後
    return;
  }

  if(node->type == Node::Type::IfElse) {
    std::string if_id = "if_id";
    gen_expr(node->ctrl_cond());                // 条件式を評価
    code.breq("t0", "zero", if_id + "_else");   // 分岐
    gen_expr(node->ctrl_true());                // true の処理
    code.jump("zero", "zero", if_id + "_end");  // else を飛ばす
    code.label(if_id + "_else");                // else:
    gen_expr(node->ctrl_false());               // else の処理
    code.label(if_id + "_end");                 // if - else の最後
    return;
  }

  if(node->type == Node::Type::While) {
    std::string while_id = "while_id";
    code.label(while_id + "_continue");
    gen_expr(node->ctrl_cond());                   // 条評価
    code.breq("t0", "zero", while_id + "_break");  // 分岐
    gen_expr(node->ctrl_body());                   // ループ処理
    code.jump("zero", "zero", while_id + "_continue");
    code.label(while_id + "_break");
    return;
  }

  if(node->type == Node::Type::DoWhile) {
    std::string do_while_id = "do_while_id";
    code.label(do_while_id + "_continue");
    gen_expr(node->ctrl_body());                           // ループ処理
    gen_expr(node->ctrl_cond());                           // 条件評価
    code.breq("t0", "zero", do_while_id + "_break");       // 分岐
    code.jump("zero", "zero", do_while_id + "_continue");  // ループを続ける
    code.label(do_while_id + "_break");
    return;
  }

  if(node->type == Node::Type::For) {
    std::string for_id = "for_id";
    gen_expr(node->ctrl_init());  // 初期化処理
    code.label(for_id + "_check");
    gen_expr(node->ctrl_cond());                 // 条件評価
    code.breq("t0", "zero", for_id + "_break");  // 分岐
    gen_expr(node->ctrl_body());                 // ループ処理
    code.label(for_id + "_continue");
    gen_expr(node->ctrl_iterate());                // 反復処理
    code.jump("zero", "zero", for_id + "_check");  // 条件確認に戻る
    code.label(for_id + "_break");
    return;
  }

  code.comment("Cannot Generate Asm");
}

// void CodeGen::gen_for(Node* node) {
//   if(node->type == Node::Type::For) {
//     std::string for_id = "for_id";
//     gen(node->ctrl_init());  // 初期化処理
//     code.label(for_id + "_check");
//     gen(node->ctrl_cond());                      // 条件評価
//     code.breq("t0", "zero", for_id + "_break");  // 分岐
//     gen(node->ctrl_body());                      // ループ処理
//     code.label(for_id + "_continue");
//     gen(node->ctrl_iterate());                     // 反復処理
//     code.jump("zero", "zero", for_id + "_check");  // 条件確認に戻る
//     code.label(for_id + "_break");
//     return;
//   }
// }

uint16_t CodeGen::gen_global(uint16_t global_top) {
  for(auto var : symbols->symbols) {
    if(var.kind == GlobalSymbol::Kind::GVar) {
      code.addr_label(var.name, global_top);
      global_top += type_size(var.type);
      // Debug
      std::stringstream ss;
      ss << type_size(var.type) << " : " << var.type;
      code.comment(ss.str());
    }
  }
  return global_top;
}

uint16_t CodeGen::type_size(Node* node) {
  if(node->type_is(Node::Type::TypePrim)) {
    std::string type_name = node->def_name()->str;

    // 整数型 : 1
    if(type_name == "int") return 1;

    // 定義された型 : シンボルテーブルから探す
    GlobalSymbol* defined_type = symbols->find(type_name);
    if(defined_type == nullptr)
      error("Cannot find def of type:" + node->str);
    return type_size(defined_type->type);
  }

  // ポインタ型 : 1
  if(node->type_is(Node::Type::TypePointer)) {
    return 1;
  }

  // 配列型 : ベース型の N 倍
  if(node->type_is(Node::Type::TypeArray)) {
    return type_size(node->type_base()) * node->array_len();
  }

  // 構造体 : メンバ型の合計
  if(node->type_is(Node::Type::TypeStruct)) {
    int ret = 0;
    for(auto t : node->type_members())
      ret += type_size(t.second);
    return ret;
  }

  // 関数型 : RAM上になし
  if(node->type_is(Node::Type::TypeFunc)) {
    return 0;
  }
  error("This node is not type: ");
  return 0;
}
