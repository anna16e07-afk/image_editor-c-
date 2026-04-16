#pragma once

#include "filter.h"

class GlassDistortion : public Filter {
public:
    explicit GlassDistortion(double scale);

    Image Apply(const Image& image) const override;

private:
    double scale_;
    static double Noise(int x, int y);
    static double SmoothNoise(double x, double y);
};