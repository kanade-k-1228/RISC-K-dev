#include "../rkarch/arch.hpp"
#include "class/line.hpp"
#include "utils.hpp"
#include "utils/args.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <sstream>
#include <unistd.h>
#include <vector>

template <class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
  std::copy(v.begin(), v.end(), std::ostream_iterator<T>(os, " "));
  return os;
}

int main(int argc, char* argv[]) {
  auto [input_files, output_file] = parse_arg(argc, argv);

  std::cout << "----------------------------------------------------" << std::endl
            << "Assemble: " << input_files << std::endl;

  // アセンブリファイルを順番にスキャン
  std::vector<Line> stmts;     // コードリスト
  uint16_t operation_cnt = 0;  // 機械語命令カウンタ
  for(auto input_file : input_files) {
    // ファイルを開く
    std::ifstream fin(input_file, std::ios::in);
    if(!fin) {
      std::cout << "Cannot open input file: " << input_file << std::endl;
      exit(EXIT_FAILURE);
    }
    // 一行ずつスキャンし、命令リストに格納
    std::cout << " 1. Parse: " << input_file << std::endl;
    std::string line;
    for(int line_cnt = 1; std::getline(fin, line); ++line_cnt) {
      try {
        Line stmt(input_file, line_cnt, line, operation_cnt);  // 行を解釈
        stmts.push_back(stmt);                                 // 行を追加
        if(stmt.isOperation()) ++operation_cnt;                // 命令行の場合、PCのカウントアップ
      } catch(std::string* msg) {
        std::cout << print_error(input_file, line_cnt, line, *msg);
      }
    }
  }

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
      std::cout << stmt.printError(*msg);
    }
  }

  // バイナリを生成
  std::cout << " 4. Generate binary" << std::endl;
  for(auto& stmt : stmts) {
    try {
      if(stmt.isOperation()) {
        stmt.getOperation().genBin();
      }
    } catch(std::string* msg) {
      std::cout << stmt.printError(*msg);
    }
  }

  // エラーが見つかったらコードを吐かない

  // コードを出力
  std::cout << " 5. Output file : " << output_file << std::endl;
  std::ofstream fout(output_file, std::ios::out | std::ios::binary | std::ios::trunc);
  if(!fout) {
    std::cout << "Can't open file: " << output_file << std::endl;
    return EXIT_FAILURE;
  }
  for(auto& stmt : stmts) {
    if(stmt.isOperation()) {
      uint32_t bin = stmt.getOperation().getBin();
      fout.write((char*)&bin, sizeof(bin));
    }
  }
  std::cout << "----------------------------------------------------" << std::endl;

  // アセンブラを表示
  for(auto stmt : stmts) std::cout << stmt.print_pretty() << std::endl;
  std::cout << "----------------------------------------------------" << std::endl;

  // フォーマット出力
  // for(auto stmt : stmts) std::cout << stmt.print_format() << std::endl;
  // std::cout << "----------------------------------------------------" << std::endl;

  return EXIT_SUCCESS;
}
