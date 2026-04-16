#include "bmp_reader.h"

#include <fstream>
#include <stdexcept>
#include <cstdint>
#include <vector>
#include <cmath>

#pragma pack(push, 1)

constexpr uint16_t SignatureValue = 0x4D42;
constexpr uint32_t HeaderSizeBitmapinfoheader = 40;
constexpr uint16_t BitsPerPixel24 = 24;

struct BmpFileHeader {
    uint16_t signature;
    uint32_t file_size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t data_offset;
};

struct BmpInfoHeader {
    uint32_t header_size;
    int32_t width;
    int32_t height;
    uint16_t color_planes;
    uint16_t bits_per_pixel;
    uint32_t compression;
    uint32_t image_size;
    int32_t x_pixels_per_meter;
    int32_t y_pixels_per_meter;
    uint32_t colors_in_table;
    uint32_t important_colors;
};

#pragma pack(pop)

Image BmpReader::Read(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file for reading: " + path);
    }

    BmpFileHeader file_header{};
    file.read(reinterpret_cast<char*>(&file_header), sizeof(file_header));
    if (!file) {
        throw std::runtime_error("Failed to read BMP file header: " + path);
    }
    if (file_header.signature != SignatureValue) {
        throw std::runtime_error("Not a BMP file (bad signature): " + path);
    }

    BmpInfoHeader info_header{};
    file.read(reinterpret_cast<char*>(&info_header), sizeof(info_header));
    if (!file) {
        throw std::runtime_error("Failed to read BMP info header: " + path);
    }
    if (info_header.header_size != HeaderSizeBitmapinfoheader) {
        throw std::runtime_error("Unsupported DIB header (expected BITMAPINFOHEADER, size=40): " + path);
    }
    if (info_header.bits_per_pixel != BitsPerPixel24) {
        throw std::runtime_error("Only 24-bit BMP is supported: " + path);
    }
    if (info_header.compression != 0) {
        throw std::runtime_error("Compressed BMP is not supported: " + path);
    }
    if (info_header.width <= 0) {
        throw std::runtime_error("Invalid image width in: " + path);
    }

    const size_t width = static_cast<size_t>(info_header.width);
    const size_t height = static_cast<size_t>(std::abs(info_header.height));
    const bool top_down = info_header.height < 0;

    if (width == 0 || height == 0) {
        throw std::runtime_error("Zero-size image in: " + path);
    }

    const size_t row_stride = (width * 3 + 3) / 4 * 4;

    file.seekg(file_header.data_offset, std::ios::beg);
    if (!file) {
        throw std::runtime_error("Failed to go to pixel data in: " + path);
    }

    Image image(width, height);
    std::vector<uint8_t> row_buf(row_stride);
    for (size_t row = 0; row < height; ++row) {
        file.read(reinterpret_cast<char*>(row_buf.data()), static_cast<std::streamsize>(row_stride));
        if (!file) {
            throw std::runtime_error("Unexpected end of file while reading pixel data: " + path);
        }

        const size_t y = top_down ? row : height - 1 - row;

        for (size_t x = 0; x < width; ++x) {
            const uint8_t b = row_buf[x * 3 + 0];
            const uint8_t g = row_buf[x * 3 + 1];
            const uint8_t r = row_buf[x * 3 + 2];
            image.SetPixel(x, y, Color(r, g, b));
        }
    }

    return image;
}