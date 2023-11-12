#pragma once
#include "comment.hpp"
#include "imm.hpp"
#include "label.hpp"
#include "operation.hpp"
#include "utils/utils.hpp"
#include <optional>
#include <sstream>
#include <string>
#include <tuple>
#include <variant>
#include <vector>

class ASMLine {
  Position position;
  std::string str;
  std::variant<std::monostate, Operation, Label> content;
  std::optional<Comment> comment;
public:
  ASMLine(const Position position, const std::string str, const uint16_t pc);
  bool isVoid() { return std::holds_alternative<std::monostate>(content); }
  bool isOperation() { return std::holds_alternative<Operation>(content); }
  bool isLabel() { return std::holds_alternative<Label>(content); }
  Operation& getOperation() { return std::get<Operation>(content); }
  Label& getLabel() { return std::get<Label>(content); }
  std::string printError(std::string msg) { return print_error(position, str, msg); }
  friend std::string print(ASMLine&);
};
