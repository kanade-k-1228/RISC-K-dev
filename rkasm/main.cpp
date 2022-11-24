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

std::string print_binary(Code&);
std::string print_asm(Code&);

int main(int argc, char* argv[]) {
  bool debug = false;

  // コマンドライン引数の処理
  opterr = 0;
  for(int opt; (opt = getopt(argc, argv, "d")) != -1;) {
    switch(opt) {
    case 'd':
      debug = true;
      break;
    default:
      std::cout << "Usage: rkasm [-d] file" << std::endl;
      break;
    }
  }
  // ファイル
  std::string fname = argv[optind];
  std::ifstream fin;
  fin.open(fname, std::ios::in);
  if(!fin) error("Can't open input file");

  LabelTable pc_lab;         // プログラムラベル
  LabelTable var_lab;        // 変数ラベル
  std::vector<Code> codes;   // コード
  uint16_t program_cnt = 0;  // 機械語命令カウンタ

  for(std::string line; std::getline(fin, line);) {
    line = trim_comment(line);                 // コメント削除
    if(is_empty(line)) continue;               // 空行削除
    if(debug) std::cout << "\r" << line;       // デバッグ用出力
    Code code(program_cnt, split(line, ' '));  // 行を解釈
    codes.push_back(code);                     // 行を追加

    if(code.opr_lab_def)                                       // ラベル定義の場合
      pc_lab.define(code.label_target_name, program_cnt);      // ラベルの追加
    else if(code.var_lab_def)                                  // ラベル定義の場合
      var_lab.define(code.label_reference_name, program_cnt);  // ラベルの追加
    else                                                       // 命令の場合
      program_cnt++;                                           // PCのカウントアップ
  }

  // ラベルの解決
  for(auto& code : codes) {
    if(code.is_label_reference) {
      uint16_t address = pc_lab.use(code.label_reference_name);
      code.label_target_value = address;
      code.imm = address;
    }
    if(code.opr_lab_def) {
      uint16_t address = pc_lab.use(code.label_target_name);
      code.label_target_value = address;
    }
  }

  // コードを出力
  std::ofstream fout;
  fout.open(fname + ".bin", std::ios::out | std::ios::binary | std::ios::trunc);
  if(!fout) error("Can't open file: " + fname + ".bin");
  for(auto& code : codes) {
    if(!code.opr_lab_def) {
      uint32_t bin = code.get_bin();
      fout.write((char*)&bin, sizeof(bin));
    }
  }

  // 表示
  if(debug) std::cout << std::endl;
  std::cout << "------------------------------------" << std::endl
            << "Assembly: " << fname << std::endl
            << "------------------------------------" << std::endl;
  for(auto var : var_lab) {
    std::cout << cprint(hex(true, var.second) + " " + var.first, YELLOW, 0) << std::endl;
  }
  for(auto code : codes) {
    if(code.opr_lab_def) {
      std::cout << cprint(code.label_target_name + ":" + hex(true, code.label_target_value), GREEN, 0) << std::endl;
    } else if(code.var_lab_def) {
    } else {
      std::cout << hex(true, code.address) << " |";
      if(debug) std::cout << " " << print_binary(code) << " |";
      std::cout << print_asm(code) << std::endl;
    }
  }
  return EXIT_SUCCESS;
}

std::string print_binary(Code& code) {
  std::stringstream ss;
  ss << std::bitset<6>((code.code >> 26) & 0x3f) << " "
     << std::bitset<6>((code.code >> 20) & 0x3f) << " "
     << std::bitset<10>((code.code >> 10) & 0x3ff) << " "
     << std::bitset<4>((code.code >> 6) & 0xf) << " "
     << std::bitset<6>(code.code & 0x3f);
  return ss.str();
}

std::string print_asm(Code& code) {
  std::stringstream ss;
  ss << cprint(code.code_s.at(0), RED, 6);
  if(code.op == CALC)
    ss << cprint(code.code_s.at(1), BLUE, 6)
       << cprint(code.code_s.at(2), BLUE, 8)
       << cprint(code.code_s.at(3), BLUE, 8);
  if(code.op == CALCI)
    ss << cprint(code.code_s.at(1), BLUE, 6)
       << cprint(code.code_s.at(2), BLUE, 8)
       << cprint(hex(true, code.imm), YELLOW, 8);
  if(code.op == LOAD)
    ss << cprint(code.code_s.at(1), BLUE, 6)
       << cprint(code.code_s.at(2), BLUE, 8)
       << cprint(hex(true, code.imm), YELLOW, 8);
  if(code.op == LOADI)
    ss << cprint(code.code_s.at(1), BLUE, 6)
       << (code.is_label_reference
               ? cprint(code.code_s.at(2), GREEN, 8)
               : cprint(hex(true, code.imm), YELLOW, 8));
  if(code.op == STORE)
    ss << cprint(code.code_s.at(1), BLUE, 6)
       << cprint(code.code_s.at(2), BLUE, 8)
       << cprint(hex(true, code.imm), YELLOW, 8);
  if(code.op == JUMP)
    ss << cprint(code.code_s.at(1), BLUE, 6)
       << cprint(code.code_s.at(2), BLUE, 8)
       << (code.is_label_reference
               ? cprint(code.code_s.at(3), GREEN, 8)
               : cprint(hex(true, code.imm), YELLOW, 8));
  if(code.op == BREQ || code.op == BRLT)
    ss << cprint(code.code_s.at(1), BLUE, 6)
       << cprint(code.code_s.at(2), BLUE, 8)
       << (code.is_label_reference
               ? cprint(code.code_s.at(3), GREEN, 8)
               : cprint(hex(true, code.imm), YELLOW, 8));
  return ss.str();
}
