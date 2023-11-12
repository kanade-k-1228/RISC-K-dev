#pragma once
#include <string>
#include <tuple>
#include <vector>

std::tuple<std::vector<std::string>, std::string> parse_arg(int argc, char* argv[]);
