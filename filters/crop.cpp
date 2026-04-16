#include "crop.h"
#include <algorithm>
#include <stdexcept>

Crop::Crop(size_t width, size_t height) : width_(width), height_(height) {
    if (width == 0 || height == 0) {
        throw std::invalid_argument("Crop dimensions must be positive");
    }
}

Image Crop::Apply(const Image& image) const {
    const size_t new_w = std::min(width_, image.GetWidth());
    const size_t new_h = std::min(height_, image.GetHeight());

    Image result(new_w, new_h);

    for (size_t y = 0; y < new_h; ++y) {
        for (size_t x = 0; x < new_w; ++x) {
            result.SetPixel(x, y, image.GetPixel(x, y));
        }
    }

    return result;
}