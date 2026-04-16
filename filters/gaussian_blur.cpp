#include "gaussian_blur.h"
#include <cmath>
#include <stdexcept>

constexpr double Wtf = 2.0;

GaussianBlur::GaussianBlur(double sigma) : sigma_(sigma) {
    if (sigma <= 0.0) {
        throw std::invalid_argument("GaussianBlur sigma must be positive");
    }
}

std::vector<double> GaussianBlur::BuildKernel() const {
    const int radius = static_cast<int>(std::ceil(3.0 * sigma_));
    const int size = 2 * radius + 1;

    std::vector<double> kernel(static_cast<size_t>(size));
    double sum = 0.0;

    for (int i = -radius; i <= radius; ++i) {
        double v = std::exp(-(i * i) / (Wtf * sigma_ * sigma_));
        kernel[static_cast<size_t>(i + radius)] = v;
        sum += v;
    }
    for (double& v : kernel) {
        v /= sum;
    }

    return kernel;
}

Image GaussianBlur::ApplyHorizontal(const Image& image, const std::vector<double>& kernel) {
    const int radius = static_cast<int>(kernel.size()) / 2;
    const size_t w = image.GetWidth();
    const size_t h = image.GetHeight();
    Image result(w, h);

    for (size_t y = 0; y < h; ++y) {
        for (size_t x = 0; x < w; ++x) {
            double r = 0.0;
            double g = 0.0;
            double b = 0.0;
            for (int dx = -radius; dx <= radius; ++dx) {
                double weight = kernel[static_cast<size_t>(dx + radius)];
                Color c = image.GetPixelValid(static_cast<int>(x) + dx, static_cast<int>(y));
                r += weight * c.r;
                g += weight * c.g;
                b += weight * c.b;
            }
            result.SetPixel(x, y, Color::FromDouble(r, g, b));
        }
    }

    return result;
}

Image GaussianBlur::ApplyVertical(const Image& image, const std::vector<double>& kernel) {
    const int radius = static_cast<int>(kernel.size()) / 2;
    const size_t w = image.GetWidth();
    const size_t h = image.GetHeight();
    Image result(w, h);

    for (size_t y = 0; y < h; ++y) {
        for (size_t x = 0; x < w; ++x) {
            double r = 0.0;
            double g = 0.0;
            double b = 0.0;
            for (int dy = -radius; dy <= radius; ++dy) {
                double weight = kernel[static_cast<size_t>(dy + radius)];
                Color c = image.GetPixelValid(static_cast<int>(x), static_cast<int>(y) + dy);
                r += weight * c.r;
                g += weight * c.g;
                b += weight * c.b;
            }
            result.SetPixel(x, y, Color::FromDouble(r, g, b));
        }
    }
    return result;
}

Image GaussianBlur::Apply(const Image& image) const {
    const std::vector<double> kernel = BuildKernel();
    return ApplyVertical(ApplyHorizontal(image, kernel), kernel);
}