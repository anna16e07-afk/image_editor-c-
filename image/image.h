#pragma once

#include "color.h"
#include <vector>
#include <cstddef>

class Image {
public:
    Image() = default;
    Image(size_t width, size_t height);

    size_t GetWidth() const;
    size_t GetHeight() const;

    Color GetPixel(size_t x, size_t y) const;
    void SetPixel(size_t x, size_t y, Color color);

    Color GetPixelValid(int x, int y) const;

private:
    size_t width_ = 0;
    size_t height_ = 0;
    std::vector<std::vector<Color>> pixels_;
};