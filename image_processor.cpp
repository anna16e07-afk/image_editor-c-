#include "bmp/bmp_reader.h"
#include "bmp/bmp_writer.h"
#include "parser/parser.h"
#include "filters/filter_factory.h"
#include "pipeline/pipeline.h"

#include <iostream>

int main(int argc, char** argv) {
    try {
        const AppOptions opts = Parser::Parse(argc, argv);

        Image image = BmpReader::Read(opts.input_path);

        Pipeline pipeline;
        for (const FilterRequest& req : opts.filters) {
            pipeline.AddFilter(FilterFactory::Create(req));
        }

        const Image result = pipeline.Run(image);

        BmpWriter::Write(opts.output_path, result);

    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}