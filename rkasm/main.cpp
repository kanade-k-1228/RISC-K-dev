#include "../rkisa/rkisa.hpp"
#include "../utils/utils.hpp"
#include "code.hpp"
#include "label.hpp"
#include "reader.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <regex>
#include <sstream>
#include <unistd.h>
#include <vector>

std::string trim_comment(std::string);
bool is_empty(const std::string&);
std::string print_binary(uint32_t);

std::string man
    = "rkasm [-b] [-w] [-c] [-v] .rkasm\n"
      "  -b: print Binary     バイナリを表示\n"
      "  -w: print Warning    警告を表示\n"
      "  -c: print Const list 定数リストを表示\n"
      "  -v: print Var list   変数リストを表示\n"
      "FILE: asembly file     アセンブリファイル";

int main(int argc, char* argv[]) {
  bool opt_b = false;
  bool opt_w = false;
  bool opt_c = false;
  bool opt_v = false;

  // コマンドライン引数の処理
  opterr = 0;
  for(int opt; (opt = getopt(argc, argv, "bwcv")) != -1;) {
    if(opt == 'b') opt_b = true;
    if(opt == 'w') opt_w = true;
    if(opt == 'c') opt_c = true;
    if(opt == 'v') opt_v = true;
    if(opt == '?') {
      std::cout << man << std::endl;
      return EXIT_FAILURE;
    }
  }
  if(!(optind < argc)) {
    std::cout << man << std::endl;
    return EXIT_FAILURE;
  }
  std::string fname = argv[optind];
  std::cout << "------------------------------------" << std::endl
            << "Assemble: " << fname << std::endl
            << "------------------------------------" << std::endl;

  // 一行ずつスキャンし、命令リストに格納
  std::vector<ASMLine> asmlines;  // コードリスト
  uint16_t operation_cnt = 0;     // 機械語命令カウンタ
  for(Reader::init(fname); Reader::getline();) {
    std::string line = trim_comment(Reader::line);        // コメント削除
    if(is_empty(line)) continue;                          // 空行ならスキップ
    ASMLine code(operation_cnt, split(line, ' '));        // 行を解釈
    asmlines.push_back(code);                             // 行を追加
    if(code.type == ASMLine::OPERATION) ++operation_cnt;  // 命令行の場合、PCのカウントアップ
  }

  // ラベルテーブルの生成
  LabelTable opr_lab;    // 命令ラベルテーブル
  LabelTable var_lab;    // 変数ラベルテーブル
  LabelTable const_lab;  // 定数ラベルテーブル
  for(auto& asmline : asmlines) {
    if(asmline.type == ASMLine::LABEL_DEF) {
      if(asmline.label.type == LabelDef::OPR)
        opr_lab.define(asmline.label.name, asmline.label.value);
      if(asmline.label.type == LabelDef::VAR)
        var_lab.define(asmline.label.name, asmline.label.value);
      if(asmline.label.type == LabelDef::CONST)
        const_lab.define(asmline.label.name, asmline.label.value);
    }
  }

  // ラベルの解決
  for(auto& code : asmlines) {
    if(code.type == ASMLine::OPERATION) {
      if(opt_b) std::cout << "\r" << code.opr.print();
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

  // コードを出力
  std::ofstream fout;
  fout.open(fname + ".bin", std::ios::out | std::ios::binary | std::ios::trunc);
  if(!fout) error("Can't open file: " + fname + ".bin");
  for(auto& code : asmlines) {
    if(code.type == ASMLine::OPERATION) {
      uint32_t bin = code.opr.get_bin();
      fout.write((char*)&bin, sizeof(bin));
    }
  }

  // 表示
  if(opt_b) std::cout << "\r";
  // ラベルの一覧表示
  if(opt_c) {
    for(auto var : const_lab.sort_by_value())
      std::cout << cprint(hex(true, var.first), YELLOW, 0) << " == " << var.second << std::endl;
    std::cout << "------------------------------------" << std::endl;
  }
  if(opt_v) {
    for(auto var : var_lab.sort_by_value())
      std::cout << cprint(hex(true, var.first), BLUE, 0) << " <- " << var.second << std::endl;
    std::cout << "------------------------------------" << std::endl;
  }
  // アセンブラを表示
  for(auto code : asmlines) {
    if(code.type == ASMLine::LABEL_DEF && code.label.type == LabelDef::OPR) {
      std::cout << cprint(hex(true, code.label.value), GREEN, 0) << cprint(": " + code.label.name, GREEN, 0) << std::endl;
    }
    if(code.type == ASMLine::OPERATION) {
      std::cout << hex(true, code.opr.addr) << " |";
      if(opt_b) std::cout << " " << print_binary(code.opr.bin) << " |";
      std::cout << code.opr.print() << std::endl;
    }
  }
  return EXIT_SUCCESS;
}


std::string trim_comment(std::string str) {
  const auto pos = str.find(";");  // 最初にヒットした ; の位置
  if(pos != std::string::npos)     // npos はヒットしなかった場合の pos の値
    str = str.erase(pos);          // pos 以降を削除
  return str;
}

bool is_empty(const std::string& str) {
  const std::regex non_empty(R"(\S)");  // 空でない文字にヒットする正規表現
  return !std::regex_search(str, non_empty);
}

std::string print_binary(uint32_t bin) {
  std::stringstream ss;
  ss << std::bitset<16>((bin >> 16) & 0xffff) << " "
     << std::bitset<4>((bin >> 12) & 0xf) << " "
     << std::bitset<4>((bin >> 8) & 0xf) << " "
     << std::bitset<4>((bin >> 4) & 0xf) << " "
     << std::bitset<4>(bin & 0xf);
  return ss.str();
}
