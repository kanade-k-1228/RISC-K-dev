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
  Type type;
  std::string label;
  void set(std::string);
};

struct Operation {
  uint32_t bin;
  uint16_t addr;
  std::vector<std::string> str;
  uint16_t op;
  uint16_t func;
  uint16_t rs1;
  uint16_t rs2;
  uint16_t rd;
  Imm imm;
  void set(const uint16_t, const std::vector<std::string>);
  uint32_t get_bin();
};

struct Label {
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

struct Code {
  enum Type {
    OPERATION,
    LABEL_DEF
  };
  Type type;
  std::vector<std::string> code_s;
  Operation opr;
  Label label;
  Code(const uint16_t address, const std::vector<std::string> code_s);
};
