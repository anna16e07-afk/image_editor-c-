#include "grayscale.h"

Image Grayscale::Apply(const Image& image) const {
    const size_t w = image.GetWidth();
    const size_t h = image.GetHeight();
    Image result(w, h);

    for (size_t y = 0; y < h; ++y) {
        for (size_t x = 0; x < w; ++x) {
            const Color c = image.GetPixel(x, y);
            const double gray = 0.299 * c.r + 0.587 * c.g + 0.114 * c.b;
            const uint8_t v = Color::ValidatesToUint8(gray);
            result.SetPixel(x, y, Color(v, v, v));
        }
    }

    return result;
}