#include "class/line.hpp"
#include "isa.hpp"
#include "utils.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <vector>

void print_error(std::string fname, int line_cnt, std::string line, std::string msg) {
  std::string place = fname + ":" + std::to_string(line_cnt);
  std::cout << std::string(place.size(), ' ') << " | " << std::endl
            << place << " | " << line << std::endl
            << std::string(place.size(), ' ') << " | \033[31m ERROR! \033[m " << msg << std::endl;
}

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

  bool error = false;

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
  if(!fin) {
    std::cout << "Cannot open input file: " << fname << std::endl;
    exit(EXIT_FAILURE);
  }

  // 一行ずつスキャンし、命令リストに格納
  std::cout << " 1. Parse" << std::endl;
  std::string line;
  std::vector<Line> stmts;     // コードリスト
  uint16_t operation_cnt = 0;  // 機械語命令カウンタ
  for(int line_cnt = 1; std::getline(fin, line); ++line_cnt) {
    try {
      Line stmt(fname, line_cnt, line, operation_cnt);  // 行を解釈
      stmts.push_back(stmt);                            // 行を追加
      if(stmt.isOperation()) ++operation_cnt;           // 命令行の場合、PCのカウントアップ
    } catch(std::string* msg) {
      print_error(fname, line_cnt, line, *msg);
      error = true;
    }
  }
  if(error) exit(EXIT_FAILURE);

  // ラベルを集める
  std::cout << " 2. Collect label" << std::endl;
  std::vector<Label> labels;
  for(auto& stmt : stmts) {
    if(stmt.isLabel()) labels.push_back(stmt.getLabel());
  }

  // ラベルの解決
  std::cout << " 3. Resolute label" << std::endl;
  for(auto& stmt : stmts) {
    try {
      if(stmt.isOperation()) {
        stmt.getOperation().resoluteLabel(labels);
      }
    } catch(std::string* msg) {
      print_error(fname, 0, "", *msg);
      error = true;
    }
  }
  if(error) exit(EXIT_FAILURE);

  // コードを出力
  std::ofstream fout;
  fout.open(fname + ".bin", std::ios::out | std::ios::binary | std::ios::trunc);
  if(!fout) {
    std::cout << "Can't open file: " << fname << ".bin" << std::endl;
    exit(EXIT_FAILURE);
  }
  for(auto& stmt : stmts) {
    if(stmt.isOperation()) {
      uint32_t bin = stmt.getOperation().getBin();
      fout.write((char*)&bin, sizeof(bin));
    }
  }
  std::cout << "\r                                                  \r";  // デバッグ出力をクリア
  std::cout << " -> " << fname + ".bin" << std::endl;
  std::cout << "--------------------------------------------------" << std::endl;

  // ラベルの一覧表示
  if(opt_c) {
    for(auto lab : labels) {
      if(lab.isConst()) {
        std::cout << lab.print() << std::endl;
      }
    }
    std::cout << "--------------------------------------------------" << std::endl;
  }
  if(opt_v) {
    for(auto lab : labels)
      if(lab.isVar()) std::cout << lab.print() << std::endl;
    std::cout << "--------------------------------------------------" << std::endl;
  }

  // アセンブラを表示
  for(auto stmt : stmts) {
    if(stmt.isLabel() && stmt.getLabel().isOpr()) {
      std::cout << stmt.getLabel().print() << std::endl;
    }
    if(stmt.isOperation()) {
      std::cout << hex(true, stmt.getOperation().address)
                << " | " << hex(true, stmt.getOperation().getBin())
                << " |" << stmt.getOperation().print() << std::endl;
    }
  }
  return EXIT_SUCCESS;
}
