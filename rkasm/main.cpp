#include "../rkisa/rkisa.hpp"
#include "../utils/utils.hpp"
#include "code.hpp"
#include "label.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <regex>
#include <sstream>
#include <unistd.h>
#include <vector>

std::string print_binary(uint32_t);
std::string print_asm(Operation&);
std::string print_imm(Imm&);

int main(int argc, char* argv[]) {
  bool print_debug = false;
  bool print_warn = false;
  bool print_const = false;
  bool print_var = false;

  // コマンドライン引数の処理
  opterr = 0;
  for(int opt; (opt = getopt(argc, argv, "dwcv")) != -1;) {
    if(opt == 'd') print_debug = true;
    if(opt == 'w') print_warn = true;
    if(opt == 'c') print_const = true;
    if(opt == 'v') print_var = true;
    if(opt == '?') std::cout << "rkasm [-d] [-w] [-c] [-v] file" << std::endl;
  }

  // ファイル
  std::string fname = argv[optind];
  std::ifstream fin;
  fin.open(fname, std::ios::in);
  if(!fin) error("Can't open input file");

  std::cout << "------------------------------------" << std::endl
            << "Assembly: " << fname << std::endl
            << "------------------------------------" << std::endl;

  LabelTable opr_lab;        // 命令ラベルテーブル
  LabelTable var_lab;        // 変数ラベルテーブル
  LabelTable const_lab;      // 定数ラベルテーブル
  std::vector<Code> codes;   // コードリスト
  uint16_t program_cnt = 0;  // 機械語命令カウンタ
  for(std::string line; std::getline(fin, line);) {
    line = trim_comment(line);                  // コメント削除
    if(is_empty(line)) continue;                // 空行削除
    if(print_debug) std::cout << "\r" << line;  // デバッグ用出力
    Code code(program_cnt, split(line, ' '));   // 行を解釈
    codes.push_back(code);                      // 行を追加

    if(code.type == Code::LABEL_DEF) {  // ラベルテーブルに追加
      if(code.label.type == Label::OPR)
        opr_lab.define(code.label.name, code.label.value);
      if(code.label.type == Label::VAR)
        var_lab.define(code.label.name, code.label.value);
      if(code.label.type == Label::CONST)
        const_lab.define(code.label.name, code.label.value);
    } else {          // 命令行の場合
      program_cnt++;  // PCのカウントアップ
    }
  }

  // ラベルの解決
  for(auto& code : codes) {
    if(code.type == Code::OPERATION) {
      if(print_debug) std::cout << "\r" << print_asm(code.opr);  // デバッグ用出力
      if(code.opr.imm.type == Imm::LAB_REF) {
        std::string lab = code.opr.imm.label;
        bool lab_is_opr = opr_lab.contains(lab);
        bool lab_is_var = var_lab.contains(lab);
        bool lab_is_const = const_lab.contains(lab);

        if(lab_is_opr + lab_is_var + lab_is_const > 1)
          error("Multiple defines of label: " + lab);
        else if(lab_is_opr) {
          code.opr.imm.value = opr_lab.get_value(lab);
          code.opr.imm.type = Imm::OPR_LAB_REF;
        } else if(lab_is_var) {
          code.opr.imm.value = var_lab.get_value(lab);
          code.opr.imm.type = Imm::VAR_LAB_REF;
        } else if(lab_is_const) {
          code.opr.imm.value = const_lab.get_value(lab);
          code.opr.imm.type = Imm::CONST_LAB_REF;
        } else {
          error("Cannot find def of label: " + lab);
        }
      }
    }
  }

  // 型エラーの表示
  if(print_warn) {
    for(auto& code : codes) {
      if(code.opr.imm.type == Imm::OPR_LAB_REF && (code.opr.op == LOAD || code.opr.op == STORE))
        warn("Label type @" + cprint(hex(true, code.opr.addr), RED, 0));
      if(code.opr.imm.type == Imm::VAR_LAB_REF && (code.opr.op == JUMP || code.opr.op == BREQ || code.opr.op == BRLT))
        warn("Label type @" + cprint(hex(true, code.opr.addr), RED, 0));
    }
  }

  // コードを出力
  std::ofstream fout;
  fout.open(fname + ".bin", std::ios::out | std::ios::binary | std::ios::trunc);
  if(!fout) error("Can't open file: " + fname + ".bin");
  for(auto& code : codes) {
    if(code.type == Code::OPERATION) {
      uint32_t bin = code.opr.get_bin();
      fout.write((char*)&bin, sizeof(bin));
    }
  }

  // 表示
  if(print_debug) std::cout << "\r";
  // ラベルの一覧表示
  if(print_const) {
    for(auto var : const_lab.sort_by_value())
      std::cout << cprint(hex(true, var.first), YELLOW, 0) << " == " << var.second << std::endl;
    std::cout << "------------------------------------" << std::endl;
  }
  if(print_var) {
    for(auto var : var_lab.sort_by_value())
      std::cout << cprint(hex(true, var.first), BLUE, 0) << " <- " << var.second << std::endl;
    std::cout << "------------------------------------" << std::endl;
  }
  // アセンブラを表示
  for(auto code : codes) {
    if(code.type == Code::LABEL_DEF && code.label.type == Label::OPR) {
      std::cout << cprint(code.label.name + ":" + hex(true, code.label.value), GREEN, 0) << std::endl;
    }
    if(code.type == Code::OPERATION) {
      std::cout << hex(true, code.opr.addr) << " |";
      if(print_debug) std::cout << " " << print_binary(code.opr.bin) << " |";
      std::cout << print_asm(code.opr) << std::endl;
    }
  }
  return EXIT_SUCCESS;
}

std::string print_binary(uint32_t bin) {
  std::stringstream ss;
  ss << std::bitset<6>((bin >> 26) & 0x3f) << " "
     << std::bitset<6>((bin >> 20) & 0x3f) << " "
     << std::bitset<10>((bin >> 10) & 0x3ff) << " "
     << std::bitset<4>((bin >> 6) & 0xf) << " "
     << std::bitset<6>(bin & 0x3f);
  return ss.str();
}

std::string print_asm(Operation& code) {
  std::stringstream ss;
  ss << cprint(code.str.at(0), RED, 6);
  if(code.op == CALC) ss << cprint(code.str.at(1), BLUE, 6) << cprint(code.str.at(2), BLUE, 8) << cprint(code.str.at(3), BLUE, 8);
  if(code.op == CALCI) ss << cprint(code.str.at(1), BLUE, 6) << cprint(code.str.at(2), BLUE, 8) << print_imm(code.imm);
  if(code.op == LOAD) ss << cprint(code.str.at(1), BLUE, 6) << cprint(code.str.at(2), BLUE, 8) << print_imm(code.imm);
  if(code.op == LOADI) ss << cprint(code.str.at(1), BLUE, 6) << cprint("", BLUE, 8) << print_imm(code.imm);
  if(code.op == STORE) ss << cprint(code.str.at(1), BLUE, 6) << cprint(code.str.at(2), BLUE, 8) << print_imm(code.imm);
  if(code.op == JUMP) ss << cprint(code.str.at(1), BLUE, 6) << cprint(code.str.at(2), BLUE, 8) << print_imm(code.imm);
  if(code.op == BREQ || code.op == BRLT) ss << cprint(code.str.at(1), BLUE, 6) << cprint(code.str.at(2), BLUE, 8) << print_imm(code.imm);
  return ss.str();
}

std::string print_imm(Imm& imm) {
  if(imm.type == Imm::LITERAL) return cprint(hex(true, imm.value), YELLOW, 8);
  if(imm.type == Imm::OPR_LAB_REF) return cprint(hex(true, imm.value), GREEN, 8) + cprint(" = " + imm.label, GREEN, 0);
  if(imm.type == Imm::VAR_LAB_REF) return cprint(hex(true, imm.value), BLUE, 8) + cprint(" = " + imm.label, BLUE, 0);
  if(imm.type == Imm::CONST_LAB_REF) return cprint(hex(true, imm.value), YELLOW, 8) + cprint(" = " + imm.label, YELLOW, 0);
  return "";
}
