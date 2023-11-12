#pragma once
#include <optional>
#include <string>
#include <variant>

std::variant<std::string, int> stoios(std::string str);

std::optional<int> stoi_opt(std::string str);
