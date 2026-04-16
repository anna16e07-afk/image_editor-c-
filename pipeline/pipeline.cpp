#include "pipeline.h"

void Pipeline::AddFilter(std::unique_ptr<Filter> filter) {
    filters_.push_back(std::move(filter));
}

Image Pipeline::Run(const Image& image) const {
    Image current = image;
    for (const auto& filter : filters_) {
        current = filter->Apply(current);
    }
    return current;
}