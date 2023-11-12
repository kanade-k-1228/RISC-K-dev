#pragma once
#include "../utils.hpp"
#include "../utils/error.hpp"
#include "comment.hpp"
#include "imm.hpp"
#include "label.hpp"
#include "operation.hpp"
#include <optional>
#include <sstream>
#include <string>
#include <tuple>
#include <variant>
#include <vector>

class Line {
  Position position;
  std::string line;
  std::variant<std::monostate, Operation, Label> content;
  std::optional<Comment> comment;
public:
  Line(const Position position, const std::string str);
  bool isVoid() { return std::holds_alternative<std::monostate>(content); }
  bool isOperation() { return std::holds_alternative<Operation>(content); }
  bool isLabel() { return std::holds_alternative<Label>(content); }
  Operation& getOperation() { return std::get<Operation>(content); }
  Label& getLabel() { return std::get<Label>(content); }
  std::string print_pretty();
  // std::string print_format();
  std::string printError(std::string msg) { return print_error(position, line, msg); }
};
