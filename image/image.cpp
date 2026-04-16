#include "image.h"
#include <algorithm>

Image::Image(size_t width, size_t height) : width_(width), height_(height), pixels_(height, std::vector<Color>(width)) {
}

size_t Image::GetWidth() const {
    return width_;
}

size_t Image::GetHeight() const {
    return height_;
}

Color Image::GetPixel(size_t x, size_t y) const {
    return pixels_[y][x];
}

void Image::SetPixel(size_t x, size_t y, Color color) {
    pixels_[y][x] = color;
}

Color Image::GetPixelValid(int x, int y) const {
    const int cx = std::clamp(x, 0, static_cast<int>(width_) - 1);
    const int cy = std::clamp(y, 0, static_cast<int>(height_) - 1);
    return pixels_[static_cast<size_t>(cy)][static_cast<size_t>(cx)];
}