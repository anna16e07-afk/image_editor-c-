#pragma once

#include "filter.h"
#include <vector>

class MatrixFilter : public Filter {
public:
    using Matrix = std::vector<std::vector<double>>;

    Image Apply(const Image& image) const override;

protected:
    virtual Matrix GetMatrix() const = 0;
    virtual Image PreProcess(const Image& image) const;
};