#pragma once
#include "imm.hpp"
#include "label.hpp"
#include "operation.hpp"
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

class Line {
  enum Type {
    VOID,
    OPERATION,
    LABEL
  };
  std::string file;
  int line;
  std::string str;
  std::string comment;
  std::vector<std::string> splited;
  Type type;
  Operation operation;  // 命令文
  Label label_def;      // ラベル文
public:
  Line(const std::string file, const int line_no, const std::string str, const uint16_t pc);
  bool isOperation() { return type == Type::OPERATION; }
  bool isLabel() { return type == Type::LABEL; }
  Operation& getOperation() { return operation; }
  Label& getLabel() { return label_def; }
  std::tuple<std::string, int, std::string> getError() { return {file, line, str}; }
  std::string getStr() { return str; }
};
