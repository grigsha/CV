#pragma once

#include <string>
#include <vector>

struct ArgStructure {
    std::string name;
    std::vector<std::string> parameters;
    ArgStructure(const std::string& name, const std::vector<std::string>& parameters)
        : name(name), parameters(parameters) {
    }
};

class Args {
public:
    std::vector<ArgStructure> GetFilters() const;
    std::string GetInFile() const;
    std::string GetOutFile() const;
    void GetArgs();
    Args(int argc, char* argv[]);

private:
    std::string input_file_;
    std::string output_file_;
    std::vector<ArgStructure> filters_;
    std::vector<std::string> arguments_;
};
