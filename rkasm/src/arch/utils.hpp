#pragma once
#include "format.hpp"
#include <optional>

const std::optional<Format> get_operation(std::string mnemonic);
bool is_mnemonic(std::string str);
uint8_t reg_stoi(std::string name);
