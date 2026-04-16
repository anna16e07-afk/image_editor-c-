#include "bmp_writer.h"

#include <fstream>
#include <stdexcept>
#include <cstdint>
#include <vector>

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

void BmpWriter::Write(const std::string& path, const Image& image) {
    const size_t width = image.GetWidth();
    const size_t height = image.GetHeight();

    if (width == 0 || height == 0) {
        throw std::runtime_error("Cannot write zero-size image to: " + path);
    }

    const size_t row_stride = (width * 3 + 3) & ~size_t{3};
    const size_t pixel_data_size = row_stride * height;
    const size_t total_file_size = sizeof(BmpFileHeader) + sizeof(BmpInfoHeader) + pixel_data_size;

    BmpFileHeader file_header{};
    file_header.signature = SignatureValue;
    file_header.file_size = static_cast<uint32_t>(total_file_size);
    file_header.reserved1 = 0;
    file_header.reserved2 = 0;
    file_header.data_offset = sizeof(BmpFileHeader) + sizeof(BmpInfoHeader);

    BmpInfoHeader info_header{};
    info_header.header_size = HeaderSizeBitmapinfoheader;
    info_header.width = static_cast<int32_t>(width);
    info_header.height = static_cast<int32_t>(height);
    info_header.color_planes = 1;
    info_header.bits_per_pixel = BitsPerPixel24;
    info_header.compression = 0;
    info_header.image_size = static_cast<uint32_t>(pixel_data_size);
    info_header.x_pixels_per_meter = 0;
    info_header.y_pixels_per_meter = 0;
    info_header.colors_in_table = 0;
    info_header.important_colors = 0;

    std::ofstream file(path, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file for writing: " + path);
    }

    file.write(reinterpret_cast<const char*>(&file_header), sizeof(file_header));
    file.write(reinterpret_cast<const char*>(&info_header), sizeof(info_header));

    std::vector<uint8_t> row_buf(row_stride, 0);

    for (size_t row = 0; row < height; ++row) {
        const size_t y = height - 1 - row;

        for (size_t x = 0; x < width; ++x) {
            const Color c = image.GetPixel(x, y);
            row_buf[x * 3 + 0] = c.b;
            row_buf[x * 3 + 1] = c.g;
            row_buf[x * 3 + 2] = c.r;
        }

        file.write(reinterpret_cast<const char*>(row_buf.data()), static_cast<std::streamsize>(row_stride));
    }

    if (!file) {
        throw std::runtime_error("Failed to write BMP data to: " + path);
    }
}