#include "args.hpp"
#include <boost/program_options.hpp>
#include <iostream>

std::tuple<std::vector<std::string>, std::string> parse_arg(int argc, char* argv[]) {
  const auto version = "rkasm : RK-16 assembler : 0.0.0";
  const auto output_default = "out.rk.bin";
  const auto input_default = std::vector<std::string>{};

  namespace po = boost::program_options;

  po::options_description generic("Generic");
  generic.add_options()("version,v", "print version string")("help,h", "help message");

  po::options_description config("Config");
  config.add_options()("output,o", po::value<std::string>(), "output file");

  po::options_description hidden("Hidden");
  hidden.add_options()("input", po::value<std::vector<std::string>>(), "input files");

  po::options_description opt;
  opt.add(generic).add(config).add(hidden);

  po::positional_options_description pos;
  pos.add("input", -1);

  po::variables_map vars;
  po::store(po::command_line_parser(argc, argv).options(opt).positional(pos).run(), vars);
  po::notify(vars);

  if(vars.count("help")) {
    std::cout << opt << std::endl;
    exit(EXIT_SUCCESS);
  }

  if(vars.count("version")) {
    std::cout << version << std::endl;
    exit(EXIT_SUCCESS);
  }

  auto input_files = vars.count("input") ? vars["input"].as<std::vector<std::string>>() : input_default;
  auto output_file = vars.count("output") ? vars["output"].as<std::string>() : output_default;
  return {input_files, output_file};
}
