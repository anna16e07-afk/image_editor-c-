#pragma once

#include "filter.h"

class Grayscale : public Filter {
public:
    Image Apply(const Image& image) const override;
};