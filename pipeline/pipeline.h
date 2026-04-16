#pragma once

#include "../filters/filter.h"
#include <vector>
#include <memory>

class Pipeline {
public:
    void AddFilter(std::unique_ptr<Filter> filter);

    Image Run(const Image& image) const;

private:
    std::vector<std::unique_ptr<Filter>> filters_;
};