#pragma once
#include <string>
#include <vector>
struct LabelDef {
  enum Type {
    OPR,
    VAR,
    CONST
  };
  Type type;
  std::string name;
  uint16_t value;
  LabelDef(){};
  LabelDef(const uint16_t address, const std::vector<std::string> str);
};