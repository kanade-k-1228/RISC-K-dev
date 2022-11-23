#pragma once
#include "cpu.hpp"
#include <array>
#include <map>
#include <string>
#include <vector>

struct InteruptOption {
  int ino;
};

class InteruptPoints : public std::map<int, InteruptOption> {
public:
  bool use = false;
  std::string fname;
  void init(std::string);
  bool contain(int);
};

struct DumpOption {
  std::vector<uint16_t> address;
};

class DumpPoints : public std::map<uint16_t, DumpOption> {
public:
  bool use = false;
  std::string fname;
  void init(std::string);
  bool contain(uint16_t);
};

std::string dump(CPU&);
std::string dump(CPU&, DumpOption&);
std::string print_code(uint32_t);
