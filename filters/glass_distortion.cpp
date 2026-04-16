#include "glass_distortion.h"
#include <cmath>
#include <stdexcept>

constexpr int LargePrime1 = 1619;
constexpr int LargePrime2 = 31337;
constexpr int Size = 13;
constexpr int LargePrime3 = 15731;
constexpr int LargePrime4 = 789221;
constexpr int LargePrime5 = 1376312589;
constexpr int Mask = 0x7fffffff;
constexpr double LargePrime51 = 1376312589.0;

GlassDistortion::GlassDistortion(double scale) : scale_(scale) {
    if (scale <= 0.0) {
        throw std::invalid_argument("GlassDistortion scale must be positive");
    }
}

double GlassDistortion::Noise(int x, int y) {
    int n = x * LargePrime1 + y * LargePrime2;
    n = (n << Size) ^ n;
    int hash = (n * (n * n * LargePrime3 + LargePrime4) + LargePrime5) & Mask;
    return 1.0 - (static_cast<double>(hash) / LargePrime51);
}

double GlassDistortion::SmoothNoise(double x, double y) {
    const int ix = static_cast<int>(std::floor(x));
    const int iy = static_cast<int>(std::floor(y));
    const double fx = x - ix;
    const double fy = y - iy;

    const double ux = fx * fx * (3.0 - 2.0 * fx);
    const double uy = fy * fy * (3.0 - 2.0 * fy);

    const double n00 = Noise(ix, iy);
    const double n10 = Noise(ix + 1, iy);
    const double n01 = Noise(ix, iy + 1);
    const double n11 = Noise(ix + 1, iy + 1);

    return n00 * (1 - ux) * (1 - uy) + n10 * ux * (1 - uy) + n01 * (1 - ux) * uy + n11 * ux * uy;
}

Image GlassDistortion::Apply(const Image& image) const {
    const size_t w = image.GetWidth();
    const size_t h = image.GetHeight();
    Image result(w, h);

    constexpr double Freq = 1.0 / 32.0;

    for (size_t y = 0; y < h; ++y) {
        for (size_t x = 0; x < w; ++x) {
            const double nx = SmoothNoise(static_cast<double>(x) * Freq, static_cast<double>(y) * Freq);
            const double ny = SmoothNoise(static_cast<double>(x) * Freq + 47.3, static_cast<double>(y) * Freq + 91.7);

            const int src_x = static_cast<int>(x) + static_cast<int>(nx * scale_);
            const int src_y = static_cast<int>(y) + static_cast<int>(ny * scale_);

            result.SetPixel(x, y, image.GetPixelValid(src_x, src_y));
        }
    }

    return result;
}