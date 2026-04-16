#pragma once

#include <string>
#include <vector>

struct FilterRequest {
    std::string name;
    std::vector<std::string> params;
};

struct AppOptions {
    std::string input_path;
    std::string output_path;
    std::vector<FilterRequest> filters;
};

class Parser {
public:
    static AppOptions Parse(int argc, char** argv);

private:
    static void PrintHelp(const char* program_name);
};