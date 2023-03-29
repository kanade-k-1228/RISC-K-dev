#pragma once
#include <string>
#include <vector>

struct Imm {
  enum Type {
    LITERAL,
    LAB_REF,
    OPR_LAB_REF,
    VAR_LAB_REF,
    CONST_LAB_REF
  };
  uint16_t value;
  Type type = LITERAL;
  std::string label;
  void set(std::string);
  std::string print();
};

struct PseudoOperation {
};

struct Operation {
  uint32_t bin;
  uint16_t addr;
  std::vector<std::string> str;
  uint16_t opc;
  uint16_t func;
  uint16_t rs1;
  uint16_t rs2;
  uint16_t rd;
  Imm imm;
  void set(const uint16_t, const std::vector<std::string>);
  uint32_t get_bin();
  std::string print();
};

struct LabelDef {
  enum Type {
    OPR,
    VAR,
    CONST
  };
  Type type;
  std::string name;
  uint16_t value;
  void set(const uint16_t, const std::vector<std::string>);
};

struct ASMLine {
  enum Type {
    OPERATION,
    LABEL_DEF
  };
  Type type;
  std::vector<std::string> code_s;  // コードの文字列

  Operation opr;   // 命令行
  LabelDef label;  // ラベル行
  ASMLine(const uint16_t address, const std::vector<std::string> code_s);
};
