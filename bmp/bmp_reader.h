#pragma once

#include "../image/image.h"
#include <string>

class BmpReader {
public:
    static Image Read(const std::string& path);
};