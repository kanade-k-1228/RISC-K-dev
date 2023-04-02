#pragma once
#include "imm.hpp"
#include "label_def.hpp"
#include "operation.hpp"
#include <string>
#include <vector>
struct Line {
  enum Type {
    OPERATION,
    LABEL_DEF
  };
  Type type;
  Operation operation;  // 命令行
  LabelDef label_def;   // ラベル行
  Line(const uint16_t address, const std::vector<std::string> str);
};
