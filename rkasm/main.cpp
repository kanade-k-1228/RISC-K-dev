#include "code.hpp"
#include "utils.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <unistd.h>
#include <vector>

int main(int argc, char* argv[]) {
  // コマンドライン引数の処理
  opterr = 0;
  bool debug = false;
  std::string opt_f = "";
  for(int opt; (opt = getopt(argc, argv, "bf:")) != -1;) {
    switch(opt) {
    case 'b':
      debug = true;
      break;
    case 'f':
      opt_f = optarg;
      break;
    default:
      std::cout << "Usage: rkasm [-b] [-f fname] file" << std::endl;
      break;
    }
  }
  // ファイル
  std::string fname = argv[optind];
  std::ifstream fin;
  fin.open(fname, std::ios::in);
  if(!fin) error("Can't open input file");

  std::map<std::string, uint16_t> labels;  // ラベルとアドレスの対応
  std::vector<Code> codes;                 // コード
  uint16_t program_cnt = 0;                // 機械語命令カウンタ
  int line_cnt = 0;
  std::string line;
  while(std::getline(fin, line)) {
    line_cnt++;
    // コメント削除
    line = trim_comment(line);
    // 空行削除
    if(is_empty(line)) continue;
    if(debug) std::cout << "\r" << line;
    // 行を解釈
    const Code code(program_cnt, split(line, ' '));
    codes.push_back(code);
    // ラベルの場合、ラベルの指す位置をリストに記録
    // 命令の場合、PCをカウントアップする。
    if(code.is_label_target) {
      if(labels.contains(code.label_target_name)) error(" Find Same Label \"" + code.label_target_name + "\"");
      labels.insert(std::map<std::string, uint16_t>::value_type(code.label_target_name, program_cnt));
    } else {
      program_cnt++;
    }
  }

  // ラベルを置き換える
  for(auto& code : codes) {
    if(code.is_label_reference) {
      const auto lab = labels.find(code.label_reference_name);
      if(lab == labels.end()) error("Cannot find def of label \"" + code.label_reference_name + "\"");
      code.label_target_value = lab->second;
      code.imm = lab->second;
    }
    if(code.is_label_target) {
      const auto lab = labels.find(code.label_target_name);
      if(lab == labels.end()) error("Cannot find def of label \"" + code.label_target_name + "\"");
      code.label_target_value = lab->second;
    }
  }

  // コードを出力
  std::ofstream fout;
  fout.open(fname + ".bin", std::ios::out | std::ios::binary | std::ios::trunc);
  if(!fout) error("Can't open file");
  for(auto& code : codes) {
    if(!code.is_label_target) {
      uint32_t bin = code.get_bin();
      fout.write((char*)&bin, sizeof(bin));
    }
  }

  // 表示
  std::cout << std::endl
            << "------------------------------------" << std::endl
            << "Assembly: " << fname << std::endl
            << "------------------------------------" << std::endl;
  program_cnt = 0;
  for(auto code : codes) std::cout << code.print(debug) << std::endl;
  return EXIT_SUCCESS;
}
