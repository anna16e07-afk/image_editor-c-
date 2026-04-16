#pragma once

#include "../image/image.h"
#include <string>

class BmpWriter {
public:
    static void Write(const std::string& path, const Image& image);
};