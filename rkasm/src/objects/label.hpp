#pragma once
#include <string>
#include <vector>

class Label {
  enum Type {
    OPR,
    VAR,
    CONST
  };
  Type type;
  std::string name;
  uint16_t value;
public:
  Label(){};
  Label(const uint16_t address, const std::vector<std::string> str);
  bool is(std::string lab) { return name == lab; }
  uint16_t getValue() { return value; }
  bool isVar() { return type == VAR; }
  bool isConst() { return type == CONST; }
  bool isOpr() { return type == OPR; }
  std::string getName() { return name; }
  friend std::string print(Label&);
};
