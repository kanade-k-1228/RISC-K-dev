#pragma once
#include "imm.hpp"
#include "label_def.hpp"
#include "operation.hpp"
#include <string>
#include <vector>

class Line {
  enum Type {
    VOID,
    OPERATION,
    LABEL
  };
  int line;
  std::string str;
  std::string comment;
  std::vector<std::string> splited;
  Type type;
  Operation operation;  // 命令文
  Label label_def;      // ラベル文
public:
  Line(const int line_no, const std::string str, const uint16_t pc);
  bool isOperation() { return type == Type::OPERATION; }
  bool isLabel() { return type == Type::LABEL; }
  Operation& getOperation() { return operation; }
  Label& getLabel() { return label_def; }
};
