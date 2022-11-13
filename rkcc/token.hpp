#pragma once
#include <regex>
#include <string>
#include <vector>

class Token {
public:
  enum class Type {
    Reserved,
    Identifier,
    Number,
    Equal,         // =
    SemiCollon,    // ;
    Plus,          // +
    Minus,         // -
    Astarisc,      // *
    Slash,         // /
    Percent,       // %
    ParOpen,       // (
    ParClose,      // )
    BracketOpen,   // <
    BracketClose,  // >
    CurlyOpen,     // {
    CurlyClose,    // }
    AngleOpen,     // <
    AngleClose,    //
  };

  Type type;
  std::string str;
  int val;
  Token(Type type, std::string str, int val);
  std::string print();
};

namespace TokenRegex {
const std::regex blank(R"(^(\s))");
const std::regex reserved(R"(^(\+|-|\*|/|\(|\)))");

// const std::regex reserved(R"(^(\+|-|\*|/|%|\(|\)|=|;))");
const std::regex number("^([0-9]+)");
const std::regex equal(R"(^(=))");
const std::regex semicolon(R"(^(;))");
const std::regex plus(R"(^(\+))");
const std::regex minus(R"(^(-))");
const std::regex astarisc(R"(^(\*))");
const std::regex slash(R"(^(/))");
const std::regex percent("^(%)");
const std::regex paropen(R"(^\()");
const std::regex parclose(R"(^\))");
}  // namespace TokenRegex

class Tokens : public std::vector<Token> {
public:
  bool consume_if_token_is(std::string);
  bool expect_token_is(std::string);
  std::string token_head();
};

void tokenize(std::string, Tokens&);
