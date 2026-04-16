#include "negative.h"

constexpr uint8_t Max = 255;

Image Negative::Apply(const Image& image) const {
    const size_t w = image.GetWidth();
    const size_t h = image.GetHeight();
    Image result(w, h);

    for (size_t y = 0; y < h; ++y) {
        for (size_t x = 0; x < w; ++x) {
            const Color c = image.GetPixel(x, y);
            result.SetPixel(x, y,
                            Color(static_cast<uint8_t>(Max - c.r), static_cast<uint8_t>(Max - c.g),
                                  static_cast<uint8_t>(Max - c.b)));
        }
    }

    return result;
}