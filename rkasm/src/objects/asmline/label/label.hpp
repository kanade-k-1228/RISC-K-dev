#pragma once
#include <string>
#include <variant>
#include <vector>

class OprLabel {
  std::string name;
  uint16_t value;
public:
  OprLabel() {}
  OprLabel(const std::vector<std::string>& splited, const uint16_t program_addr)
      : name(splited.at(0).begin(), splited.at(0).end() - 1),
        value(program_addr) {}
  std::string getName() { return name; }
  uint16_t getValue() { return value; }

  static bool match(const std::vector<std::string>& splited) {
    return splited.at(0).back() == ':';
  }
};

class VarLabel {
  std::string name;
  uint16_t value;
public:
  VarLabel(const std::vector<std::string>& splited)
      : name(splited.at(1)),
        value(std::stoi(std::string(splited.at(0).begin() + 1, splited.at(0).end()), nullptr, 0)) {}
  std::string getName() { return name; }
  uint16_t getValue() { return value; }

  static bool match(const std::vector<std::string>& splited) {
    return splited.at(0).front() == '@';
  }
};

class ConstLabel {
  std::string name;
  uint16_t value;
public:
  ConstLabel(const std::vector<std::string>& splited)
      : name(splited.at(1)),
        value(std::stoi(std::string(splited.at(0).begin() + 1, splited.at(0).end()), nullptr, 0)) {}
  std::string getName() { return name; }
  uint16_t getValue() { return value; }

  static bool match(const std::vector<std::string>& splited) {
    return splited.at(0).front() == '#';
  }
};

using Label_ = std::variant<OprLabel, VarLabel, ConstLabel>;

class Label {
  Label_ label;
public:
  Label(const std::vector<std::string>& splited, const uint16_t program_address);

  uint16_t getValue() {
    return std::visit([](auto&& l) { return l.getValue(); }, label);
  }

  std::string getName() {
    return std::visit([](auto&& l) { return l.getName(); }, label);
  }

  bool isOpr() { return std::holds_alternative<OprLabel>(label); }
  bool isVar() { return std::holds_alternative<VarLabel>(label); }
  bool isConst() { return std::holds_alternative<ConstLabel>(label); }

  friend std::string printPretty(Label&, bool);
  friend std::string printFormat(Label&);

  static bool match(std::vector<std::string> splited) {
    return OprLabel::match(splited)
           || VarLabel::match(splited)
           || ConstLabel::match(splited);
  }
};

Label& findLabel(std::vector<Label>& vec, std::string name);
