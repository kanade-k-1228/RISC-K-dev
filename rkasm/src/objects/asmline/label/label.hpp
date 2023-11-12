#pragma once
#include <string>
#include <variant>
#include <vector>

class LabelBase {
  std::string name;
  uint16_t value;
public:
};

class OprLabel : LabelBase {
public:
  static bool match(std::string str) { return str.back() == ':'; }
};

class VarLabel : LabelBase {
public:
  static bool match(std::string str) { return str.front() == '@'; }
};

class ConstLabel : LabelBase {
public:
  static bool match(std::string str) { return str.front() == '#'; }
};

using LabelWIP = std::variant<OprLabel, VarLabel, ConstLabel>;

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

  uint16_t getValue() { return value; }
  std::string getName() { return name; }

  bool is(std::string lab) { return name == lab; }

  bool isVar() { return type == VAR; }
  bool isConst() { return type == CONST; }
  bool isOpr() { return type == OPR; }

  friend std::string printPretty(Label&);
  friend std::string printFormat(Label&);

  static bool match(std::vector<std::string> splited);
};

Label& findLabel(std::vector<Label> vec, std::string name);
