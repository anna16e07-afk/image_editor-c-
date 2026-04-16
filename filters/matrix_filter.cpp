#include "matrix_filter.h"
#include <stdexcept>

Image MatrixFilter::PreProcess(const Image& image) const {
    return image;
}

Image MatrixFilter::Apply(const Image& image) const {
    const Image preprocessed = PreProcess(image);
    const Matrix matrix = GetMatrix();

    if (matrix.empty() || matrix.size() % 2 == 0) {
        throw std::logic_error("Convolution matrix must be non-empty and odd-sized");
    }

    const int half = static_cast<int>(matrix.size()) / 2;
    const size_t w = preprocessed.GetWidth();
    const size_t h = preprocessed.GetHeight();

    Image result(w, h);

    for (size_t y = 0; y < h; ++y) {
        for (size_t x = 0; x < w; ++x) {
            double sum_r = 0.0;
            double sum_g = 0.0;
            double sum_b = 0.0;

            for (int dy = -half; dy <= half; ++dy) {
                for (int dx = -half; dx <= half; ++dx) {
                    double weight = matrix[static_cast<size_t>(dy + half)][static_cast<size_t>(dx + half)];
                    Color c = preprocessed.GetPixelValid(static_cast<int>(x) + dx, static_cast<int>(y) + dy);
                    sum_r += weight * c.r;
                    sum_g += weight * c.g;
                    sum_b += weight * c.b;
                }
            }

            result.SetPixel(x, y, Color::FromDouble(sum_r, sum_g, sum_b));
        }
    }

    return result;
}