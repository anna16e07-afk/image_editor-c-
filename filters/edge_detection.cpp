#include "edge_detection.h"
#include "grayscale.h"
#include <stdexcept>

EdgeDetection::EdgeDetection(double threshold) : threshold_(threshold) {
    if (threshold < 0.0 || threshold > 1.0) {
        throw std::invalid_argument("EdgeDetection threshold must be in [0.0, 1.0]");
    }
}

MatrixFilter::Matrix EdgeDetection::GetMatrix() const {
    return {{0, -1, 0}, {-1, 4, -1}, {0, -1, 0}};
}

Image EdgeDetection::PreProcess(const Image& image) const {
    return Grayscale().Apply(image);
}

Image EdgeDetection::Apply(const Image& image) const {
    const Image convolved = MatrixFilter::Apply(image);

    const size_t w = convolved.GetWidth();
    const size_t h = convolved.GetHeight();
    Image result(w, h);

    const uint8_t thresh_byte = Color::ValidatesToUint8(threshold_ * 255.0);

    for (size_t y = 0; y < h; ++y) {
        for (size_t x = 0; x < w; ++x) {
            const uint8_t v = convolved.GetPixel(x, y).r;
            const uint8_t out = v >= thresh_byte ? 255 : 0;
            result.SetPixel(x, y, Color(out, out, out));
        }
    }

    return result;
}