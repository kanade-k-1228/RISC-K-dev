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
  Label label;          // ラベル文
public:
  Line(const std::string file, const int line_no, const std::string str, const uint16_t pc);
  bool isOperation() { return type == Type::OPERATION; }
  bool isLabel() { return type == Type::LABEL; }
  Operation& getOperation() {
    if(type != Type::OPERATION)
      throw new std::string("This line is not Operation");
    return operation;
  }
  Label& getLabel() {
    if(type != Type::LABEL)
      throw new std::string("This line is not Label");
    return label;
  }
  std::string getStr() { return str; }
};
