#pragma once
#include <string>

class Imm {
  enum Type {
    LITERAL,
    LAB_REF,
    OPR_LAB_REF,
    VAR_LAB_REF,
    CONST_LAB_REF
  };
  Type type = LITERAL;
  uint16_t value;
  std::string label;
public:
  Imm() {}
  Imm(std::string str);
  std::string print();
  bool isLabRef() { return type == LAB_REF; };
  uint16_t getValue() { return value; };
  std::string getLabel() { return label; };
  void resoluteAsOpr(uint16_t val) { type = OPR_LAB_REF, value = val; };
  void resoluteAsVar(uint16_t val) { type = VAR_LAB_REF, value = val; };
  void resoluteAsConst(uint16_t val) { type = CONST_LAB_REF, value = val; };
};
