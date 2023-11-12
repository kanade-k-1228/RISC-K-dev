#include "arch/arch.hpp"
#include "class/line.hpp"
#include "utils/utils.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <sstream>
#include <unistd.h>
#include <vector>

int main(int argc, char* argv[]) {
  auto [input_files, output_file] = parse_arg(argc, argv);

  std::cout << "----------------------------------------------------" << std::endl
            << "Assemble: " << input_files << std::endl;

  std::vector<Line> asms;
  for(auto input_file : input_files) {
    std::ifstream fin(input_file, std::ios::in);
    if(!fin) {
      std::cout << "Cannot open input file: " << input_file << std::endl;
      exit(EXIT_FAILURE);
    }
    std::cout << " 1. Parse: " << input_file << std::endl;

    int line_cnt = 0;
    uint16_t pc_cnt = 0;
    std::string line;

    while(std::getline(fin, line)) {
      line_cnt++;
      try {
        Line asmline({input_file, line_cnt}, line, pc_cnt);
        asms.push_back(asmline);
        if(asmline.isOperation()) pc_cnt++;
      } catch(std::string* msg) {
        std::cout << print_error({input_file, line_cnt}, line, *msg);
      }
    }
  }

  // ラベルを集める
  std::cout << " 2. Collect label" << std::endl;
  std::vector<Label> labels;
  for(auto& stmt : asms) {
    if(stmt.isLabel()) labels.push_back(stmt.getLabel());
  }

  // ラベルの解決
  std::cout << " 3. Resolve label" << std::endl;
  for(auto& stmt : asms) {
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
  for(auto& stmt : asms) {
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
  for(auto& stmt : asms) {
    if(stmt.isOperation()) {
      uint32_t bin = stmt.getOperation().getBin();
      fout.write((char*)&bin, sizeof(bin));
    }
  }
  std::cout << "----------------------------------------------------" << std::endl;

  // アセンブラを表示
  for(auto stmt : asms) std::cout << pprint(stmt) << std::endl;
  std::cout << "----------------------------------------------------" << std::endl;

  // フォーマット出力
  // for(auto stmt : stmts) std::cout << stmt.print_format() << std::endl;
  // std::cout << "----------------------------------------------------" << std::endl;

  return EXIT_SUCCESS;
}
