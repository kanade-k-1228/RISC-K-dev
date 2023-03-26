#pragma once
#include "cpu.hpp"
#include <array>
#include <map>
#include <set>
#include <string>
#include <vector>

struct IntrOption {
  int ino;
};

class IntrPoints : public std::map<int, IntrOption> {
public:
  bool use = false;
  std::string fname;
  void init(std::string);
  bool is_intr(uint16_t);
};

struct DumpOption {
  std::set<uint16_t> address;
  bool stack = false;
};

class DumpPoints : public std::map<uint16_t, DumpOption> {
public:
  bool use = false;
  bool all = false;
  std::string fname;
  void init(std::string);
  bool is_dump(uint16_t);
};
namespace Debug {
std::string dump(CPU&, DumpOption&);
std::string dump(CPU&);
std::string print_code(uint32_t);
}  // namespace Debug
