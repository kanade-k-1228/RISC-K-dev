#pragma once
#include <string>
#include <vector>

struct Code {
  uint16_t address;
  std::vector<std::string> code_s;
  uint32_t code;
  bool opr_lab_def;
  bool opr_lab_ref;
  bool var_lab_def;
  bool var_lab_ref;
  bool is_label_reference;
  std::string op_s;
  uint16_t op;
  uint16_t func;
  uint16_t rs1;
  uint16_t rs2;
  uint16_t rd;
  uint16_t imm;
  uint16_t label_target_value;
  uint16_t label_reference_value;
  std::string label_target_name;
  std::string label_reference_name;

  Code(const uint16_t address, const std::vector<std::string> code_s);
  void set_imm_or_label(const std::string str);
  uint32_t get_bin();
};
