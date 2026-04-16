#include "parser.h"

#include <iostream>
#include <stdexcept>
#include <string>

void Parser::PrintHelp(const char* program_name) {
    std::cout << "Usage:\n"
              << program_name << " <input.bmp> <output.bmp> [-<filter> [params]] ...\n"
              << "\n"
              << "Filters:\n"
              << "  -crop <width> <height>    Crop to given size from top-left\n"
              << "                            If size exceeds image, available part is used\n"
              << "  -gs                       Convert to grayscale\n"
              << "  -neg                      Negate all colors\n"
              << "  -sharp                    Sharpen\n"
              << "  -edge <threshold>         Edge detection, threshold in [0.0, 1.0]\n"
              << "                            Pixels above threshold -> white, others -> black\n"
              << "  -blur <sigma>             Gaussian blur, sigma > 0\n"
              << "  -glass <scale>            Glass distortion, scale > 0 (e.g. 8.0)\n"
              << "\n"
              << "Filters are applied in the order listed.\n"
              << "If no filters are given, the image is saved unchanged.\n"
              << "\n"
              << "Example:\n"
              << "  " << program_name << " input.bmp output.bmp -crop 800 600 -gs -blur 0.5\n"
              << "  " << program_name << " input.bmp output.bmp -glass 10.0 -sharp\n";
}

AppOptions Parser::Parse(int argc, char** argv) {
    if (argc < 2) {
        PrintHelp(argv[0]);
        throw std::invalid_argument("No arguments given.");
    }

    if (argc < 3) {
        throw std::invalid_argument("Expected: <input.bmp> <output.bmp> [filters].");
    }

    AppOptions opts;
    opts.input_path = argv[1];
    opts.output_path = argv[2];

    for (int i = 3; i < argc;) {
        const std::string token = argv[i];

        if (token.size() < 2 || token[0] != '-') {
            throw std::invalid_argument("Expected a filter name starting with '-', got: '" + token + "'");
        }

        FilterRequest req;
        req.name = token.substr(1);
        ++i;

        while (i < argc && argv[i][0] != '-') {
            req.params.emplace_back(argv[i]);
            ++i;
        }
        opts.filters.push_back(std::move(req));
    }
    return opts;
}