#pragma once

#include "filter.h"
#include <vector>

class GaussianBlur : public Filter {
public:
    explicit GaussianBlur(double sigma);

    Image Apply(const Image& image) const override;

private:
    double sigma_;

    std::vector<double> BuildKernel() const;

    static Image ApplyHorizontal(const Image& image, const std::vector<double>& kernel);
    static Image ApplyVertical(const Image& image, const std::vector<double>& kernel);
};