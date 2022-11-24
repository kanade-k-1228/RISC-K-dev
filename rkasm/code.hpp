#pragma once
#include <string>
#include <vector>

struct Code {
  uint16_t addr;
  std::vector<std::string> code_s;
  uint32_t code;

  std::string op_s;
  uint16_t op;
  uint16_t func;
  uint16_t rs1;
  uint16_t rs2;
  uint16_t rd;

  uint16_t imm;
  // uint16_t lab_val;
  bool opr_lab_def = false;
  bool var_lab_def = false;
  bool lab_ref = false;
  bool opr_lab_ref = false;
  bool var_lab_ref = false;
  std::string lab_str;

  Code(const uint16_t address, const std::vector<std::string> code_s);
  void set_imm_or_label(const std::string str);
  uint32_t get_bin();
};
