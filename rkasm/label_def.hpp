#pragma once
#include <string>
#include <vector>
struct Label {
  enum Type {
    OPR,
    VAR,
    CONST
  };
  Type type;
public:
  std::string name;
  uint16_t value;
  Label(){};
  Label(const uint16_t address, const std::vector<std::string> str);
  bool isVar() { return type == VAR; }
  bool isConst() { return type == CONST; }
  bool isOpr() { return type == OPR; }
};
