#include "../include/args.h"
#include <stdexcept>
#include <iostream>

static constexpr std::string_view HELP =
    "Usage:\n\n"
    "   ./image_processor {path to input file} {path to output file}\n"
    "   [-{filter name 1} [filter parameter 1] [filter parameter 2] ...]\n"
    "   [-{filter name 2} [filter parameter 1] [filter parameter 2] ...]\n"
    "   ...\n";

Args::Args(int argc, char* argv[]) {
    if (argc == 1) {
        std::cout << HELP << std::endl;
        return;
    }
    if (argc < 3) {
        throw std::invalid_argument("Not enough arguments");
    }
    input_file_ = argv[1];
    output_file_ = argv[2];
    for (size_t i = 3; i < argc;) {
        std::string filter = argv[i];
        std::vector<std::string> parametres;
        i++;
        while (i < argc && argv[i][0] != '-') {
            parametres.push_back(argv[i]);
            i++;
        }
        filters_.push_back(ArgStructure(filter, parametres));
    }
}

std::string Args::GetInFile() const {
    return input_file_;
}

std::string Args::GetOutFile() const {
    return output_file_;
}

std::vector<ArgStructure> Args::GetFilters() const {
    return filters_;
}
