#pragma once

#include "matrix_filter.h"

class EdgeDetection : public MatrixFilter {
public:
    explicit EdgeDetection(double threshold);

protected:
    Matrix GetMatrix() const override;
    Image PreProcess(const Image& image) const override;
    Image Apply(const Image& image) const override;

private:
    double threshold_;
};