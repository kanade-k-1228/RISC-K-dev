#pragma once
#include "instruction/instruction.hpp"
#include "label/label.hpp"
#include "utils/utils.hpp"
#include <optional>
#include <sstream>
#include <string>
#include <tuple>
#include <variant>
#include <vector>

struct Comment {
  std::size_t pos;
  std::string comment;
};

class ASMLine {
  Position position;
  std::string str;
  std::variant<std::monostate, Instruction, Label> content;
  std::optional<Comment> comment;
public:
  ASMLine(const Position position, const std::string str, const uint16_t pc);

  bool isVoid() { return std::holds_alternative<std::monostate>(content); }
  bool isOperation() { return std::holds_alternative<Instruction>(content); }
  bool isLabel() { return std::holds_alternative<Label>(content); }

  Instruction& getOperation() { return std::get<Instruction>(content); }
  Label& getLabel() { return std::get<Label>(content); }

  std::string printError(std::string msg) { return print_error(position, str, msg); }
  friend std::string printPretty(ASMLine&);
  friend std::string printFormat(ASMLine&);
};
