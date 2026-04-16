#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>

constexpr double Border = 255.0;

struct Color {
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;

    Color() = default;
    Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {
    }

    static uint8_t ValidatesToUint8(double v) {
        return static_cast<uint8_t>(std::clamp(std::round(v), 0.0, Border));
    }

    static Color FromDouble(double r, double g, double b) {
        return Color(ValidatesToUint8(r), ValidatesToUint8(g), ValidatesToUint8(b));
    }
};