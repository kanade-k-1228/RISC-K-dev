#include "asmgen.hpp"
#include "../utils/utils.hpp"
#include <sstream>

namespace Asm {
std::vector<std::string> code;
void comment(std::string comment) { code.push_back("; " + comment); }
void label(std::string label) { code.push_back(label + ":"); };
void add(std::string rd, std::string rs1, std::string rs2) { code.push_back("  add   " + rd + " " + rs1 + " " + rs2); }
void sub(std::string rd, std::string rs1, std::string rs2) { code.push_back("  sub   " + rd + " " + rs1 + " " + rs2); }
void addi(std::string rd, std::string rs, int imm) { code.push_back("  addi  " + rd + " " + rs + " " + std::to_string(imm)); }
void subi(std::string rd, std::string rs, int imm) { code.push_back("  subi  " + rd + " " + rs + " " + std::to_string(imm)); }
void load(std::string rd, std::string addr_base, int addr_imm) { code.push_back("  load  " + rd + " " + addr_base + " " + std::to_string(addr_imm)); }
void load(std::string rd, std::string addr_base, std::string label) { code.push_back("  load  " + rd + " " + addr_base + " " + label); }
void loadi(std::string rd, int imm) { code.push_back("  loadi " + rd + " " + std::to_string(imm)); }
void loadi(std::string rd, std::string label) { code.push_back("  loadi " + rd + " " + label); }
void store(std::string src, std::string addr_base, int addr_imm) { code.push_back("  store " + addr_base + " " + src + " " + std::to_string(addr_imm)); }
void store(std::string src, std::string addr_base, std::string label) { code.push_back("  store " + addr_base + " " + src + " " + label); }
void jump(std::string rd, std::string rs, int addr) { code.push_back("  jump  " + rd + " " + rs + " " + std::to_string(addr)); }
void jump(std::string rd, std::string rs, std::string label) { code.push_back("  jump  " + rd + " " + rs + " " + label); }
void breq(std::string rs1, std::string rs2, int addr) { code.push_back("  breq  " + rs1 + " " + rs2 + " " + std::to_string(addr)); }
void breq(std::string rs1, std::string rs2, std::string label) { code.push_back("  breq  " + rs1 + " " + rs2 + " " + label); }
// 拡張命令
void push(std::string reg) {
  store(reg, "sp", 0);
  subi("sp", "sp", 1);
}
void pop(std::string reg) {
  addi("sp", "sp", 1);
  load(reg, "sp", 0);
}
std::string print() {
  std::stringstream ss;
  for(auto line : code) ss << line << std::endl;
  return ss.str();
}
};  // namespace Asm

void asembly(Node* node) {
  if(node->type == Node::Type::Program) {
    asembly(node->lhs);
    Asm::comment("----------------");
    asembly(node->rhs);
    return;
  }
  if(node->type == Node::Type::Assignment) {
    // 代入する値を計算する
    asembly(node->rhs);
    // 値は一時レジスタ t0 に入っているので，左辺に代入する
    // 代入式の左辺はレジスタまたはメモリアドレスである必要がある
    if(node->lhs->type == Node::Type::Ident) {
      Asm::store("t0", "zero", node->lhs->str);
    }
    return;
  }

  // 三項演算
  if(node->type == Node::Type::Cond) {
    std::string cond = "cond_id";
    // 条件式を評価
    asembly(node->cond);
    // 条件値はt0に入っている
    Asm::breq("t0", "zero", cond + "_false");
    // True
    asembly(node->true_stmt);
    Asm::jump("zero", "zero", cond + "_end");
    // False
    Asm::label(cond + "_false");
    asembly(node->false_stmt);
    // End
    Asm::label(cond + "_end");
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
    asembly(node->lhs);  // 左辺値を評価し
    Asm::push("t0");     // スタックに退避
    asembly(node->rhs);  // 右辺値を評価
    Asm::pop("t1");      // 左辺値を戻す

    if(node->type == Node::Type::LogicalOr) Asm::comment("[t0 = (t0 || t1)]");
    if(node->type == Node::Type::LogicalAnd) Asm::comment("[t0 = (t0 && t1)]");

    if(node->type == Node::Type::BitOr) Asm::comment("[t0 = (t0 | t1)]");
    if(node->type == Node::Type::BitXor) Asm::comment("[t0 = (t0 ^ t1)]");
    if(node->type == Node::Type::BitAnd) Asm::comment("[t0 = (t0 & t1)]");

    if(node->type == Node::Type::EQ) Asm::comment("[t0 = (t0 == t1)]");
    if(node->type == Node::Type::NEQ) Asm::comment("[t0 = (t0 != t1)]");
    if(node->type == Node::Type::LT) Asm::comment("[t0 = (t0 < t1)]");
    if(node->type == Node::Type::LEQ) Asm::comment("[t0 = (t0 <= t1)]");
    if(node->type == Node::Type::GT) Asm::comment("[t0 = (t0 > t1)]");
    if(node->type == Node::Type::GEQ) Asm::comment("[t0 = (t0 >= t1)]");

    if(node->type == Node::Type::RShift) Asm::comment("[t0 = (t0 >> t1)]");
    if(node->type == Node::Type::LShift) Asm::comment("[t0 = (t0 << t1)]");

    if(node->type == Node::Type::Add) Asm::add("t0", "t0", "t1");
    if(node->type == Node::Type::Sub) Asm::sub("t0", "t0", "t1");

    if(node->type == Node::Type::Mul) Asm::comment("[t0 = (t0 * t1)]");
    if(node->type == Node::Type::Div) Asm::comment("[t0 = (t0 / t1)]");
    if(node->type == Node::Type::Mod) Asm::comment("[t0 = (t0 % t1)]");

    return;
  }

  if(node->type == Node::Type::UnaryAdd) {
    asembly(node->lhs);
    Asm::addi("t0", "t0", 1);
    return;
  }
  if(node->type == Node::Type::UnarySub) {
    asembly(node->lhs);
    Asm::subi("t0", "t0", 1);
    return;
  }

  // if(node->type == Node::Type::PostAdd) return "( " + print(node->lhs) + " ++ )";
  // if(node->type == Node::Type::PostSub) return "( " + print(node->lhs) + " -- )";

  if(node->type == Node::Type::Num) {
    // 定数値
    Asm::loadi("t0", node->val);
    return;
  }

  if(node->type == Node::Type::Ident) {
    // 変数の入ってるアドレスから値を取り出す
    // グローバル変数の場合
    Asm::load("t0", "zero", node->str);
    return;
    // ローカル変数の場合
    std::string addr = "XXX";
    Asm::load("t0", "fp", node->str);
    return;
  }

  // if(node->type == Node::Type::CompoundStmt) return print(node->lhs) + "\n" + print(node->rhs);

  if(node->type == Node::Type::If) {
    std::string if_id = "if_id";
    asembly(node->cond);                      // 条件式を評価
    Asm::breq("t0", "zero", if_id + "_end");  // 分岐
    asembly(node->true_stmt);                 // true の処理
    Asm::label(if_id + "_end");               // if の最後
    return;
  }

  if(node->type == Node::Type::IfElse) {
    std::string if_id = "if_id";
    asembly(node->cond);                        // 条件式を評価
    Asm::breq("t0", "zero", if_id + "_else");   // 分岐
    asembly(node->true_stmt);                   // true の処理
    Asm::jump("zero", "zero", if_id + "_end");  // else を飛ばす
    Asm::label(if_id + "_else");                // else:
    asembly(node->false_stmt);                  // else の処理
    Asm::label(if_id + "_end");                 // if - else の最後
    return;
  }

  if(node->type == Node::Type::While) {
    std::string while_id = "while_id";
    Asm::label(while_id + "_continue");
    asembly(node->cond);                           // 条評価
    Asm::breq("t0", "zero", while_id + "_break");  // 分岐
    asembly(node->stmt);                           // ループ処理
    Asm::jump("zero", "zero", while_id + "_continue");
    Asm::label(while_id + "_break");
    return;
  }

  if(node->type == Node::Type::DoWhile) {
    std::string do_while_id = "do_while_id";
    Asm::label(do_while_id + "_continue");
    asembly(node->stmt);                                   // ループ処理
    asembly(node->cond);                                   // 条件評価
    Asm::breq("t0", "zero", do_while_id + "_break");       // 分岐
    Asm::jump("zero", "zero", do_while_id + "_continue");  // ループを続ける
    Asm::label(do_while_id + "_break");
    return;
  }

  if(node->type == Node::Type::For) {
    std::string for_id = "for_id";
    asembly(node->init);  // 初期化処理
    Asm::label(for_id + "_check");
    asembly(node->cond);                         // 条件評価
    Asm::breq("t0", "zero", for_id + "_break");  // 分岐
    asembly(node->stmt);                         // ループ処理
    Asm::label(for_id + "_continue");
    asembly(node->iterate);                        // 反復処理
    Asm::jump("zero", "zero", for_id + "_check");  // 条件確認に戻る
    Asm::label(for_id + "_break");
    return;
  }

  Asm::comment("Cannot Generate Asm");
}
