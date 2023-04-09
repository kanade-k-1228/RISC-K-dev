#include "isa.hpp"
#include "label_table.hpp"
#include "line.hpp"
#include "utils.hpp"
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
    = "rkasm [-w] [-c] [-v] .rkasm\n"
      "  -w: print Warning    警告を表示\n"
      "  -c: print Const list 定数リストを表示\n"
      "  -v: print Var list   変数リストを表示\n"
      "FILE: asembly file     アセンブリファイル";

int main(int argc, char* argv[]) {
  bool opt_w = false;
  bool opt_c = false;
  bool opt_v = false;

  // コマンドライン引数の処理
  opterr = 0;
  for(int opt; (opt = getopt(argc, argv, "wcv")) != -1;) {
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
  std::cout << "--------------------------------------------------" << std::endl
            << "Assemble: " << fname << std::endl;

  // ファイルを開く
  std::ifstream fin(fname, std::ios::in);
  if(!fin) error("Can't open input file: " + fname);

  // 一行ずつスキャンし、命令リストに格納
  std::string line;
  std::vector<Line> asmlines;  // コードリスト
  uint16_t operation_cnt = 0;  // 機械語命令カウンタ
  for(int line_cnt = 1; std::getline(fin, line); ++line_cnt) {
    std::cout << "\33[2K\r" << fname << ":" << line_cnt << ":" << line;
    std::string trimmed = trim_comment(line);          // コメント削除
    if(is_empty(trimmed)) continue;                    // 空行ならスキップ
    Line code(operation_cnt, split(trimmed, ' '));     // 行を解釈
    asmlines.push_back(code);                          // 行を追加
    if(code.type == Line::OPERATION) ++operation_cnt;  // 命令行の場合、PCのカウントアップ
  }
  std::cout << "\33[2K\r";

  // ラベルテーブルの生成
  LabelTable opr_lab;    // 命令ラベルテーブル
  LabelTable var_lab;    // 変数ラベルテーブル
  LabelTable const_lab;  // 定数ラベルテーブル
  for(auto& asmline : asmlines) {
    if(asmline.type == Line::LABEL_DEF) {
      if(asmline.label_def.type == LabelDef::OPR)
        opr_lab.define(asmline.label_def.name, asmline.label_def.value);
      if(asmline.label_def.type == LabelDef::VAR)
        var_lab.define(asmline.label_def.name, asmline.label_def.value);
      if(asmline.label_def.type == LabelDef::CONST)
        const_lab.define(asmline.label_def.name, asmline.label_def.value);
    }
  }

  // ラベルの解決
  for(auto& code : asmlines) {
    if(code.type == Line::OPERATION) {
      std::cout << "\r" << code.operation.print();  // デバッグ出力
      if(code.operation.imm.type == Imm::LAB_REF) {
        std::string lab = code.operation.imm.label;
        bool lab_is_opr = opr_lab.contains(lab);
        bool lab_is_var = var_lab.contains(lab);
        bool lab_is_const = const_lab.contains(lab);
        if(lab_is_opr + lab_is_var + lab_is_const > 1)
          error("Multiple defines of label: " + lab);
        else if(lab_is_opr) {
          code.operation.imm.value = opr_lab.get_value(lab);
          code.operation.imm.type = Imm::OPR_LAB_REF;
        } else if(lab_is_var) {
          code.operation.imm.value = var_lab.get_value(lab);
          code.operation.imm.type = Imm::VAR_LAB_REF;
        } else if(lab_is_const) {
          code.operation.imm.value = const_lab.get_value(lab);
          code.operation.imm.type = Imm::CONST_LAB_REF;
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
    if(code.type == Line::OPERATION) {
      uint32_t bin = code.operation.get_bin();
      fout.write((char*)&bin, sizeof(bin));
    }
  }

  std::cout << "\r                                                  \r";  // デバッグ出力をクリア
  std::cout << "Output  : " << fname + ".bin" << std::endl;
  std::cout << "--------------------------------------------------" << std::endl;

  // ラベルの一覧表示
  if(opt_c) {
    for(auto var : const_lab.sort_by_value())
      std::cout << cprint(hex(true, var.first), YELLOW, 0) << " == " << var.second << std::endl;
    std::cout << "--------------------------------------------------" << std::endl;
  }
  if(opt_v) {
    for(auto var : var_lab.sort_by_value())
      std::cout << cprint(hex(true, var.first), BLUE, 0) << " <- " << var.second << std::endl;
    std::cout << "--------------------------------------------------" << std::endl;
  }
  // アセンブラを表示
  for(auto code : asmlines) {
    if(code.type == Line::LABEL_DEF && code.label_def.type == LabelDef::OPR) {
      std::cout << cprint(hex(true, code.label_def.value), GREEN, 0) << cprint(": " + code.label_def.name, GREEN, 0) << std::endl;
    }
    if(code.type == Line::OPERATION) {
      std::cout << hex(true, code.operation.address)
                << " | " << print_binary(code.operation.get_bin())
                << " |" << code.operation.print() << std::endl;
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
  ss.setf(std::ios::hex, std::ios::basefield);
  ss.fill('0');
  ss << "0x" << std::setw(4) << ((bin >> 16) & 0xffff)
     << "_" << std::setw(4) << (bin & 0xffff);
  return ss.str();
}
