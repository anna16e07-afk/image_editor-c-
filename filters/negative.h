#pragma once

#include "filter.h"

class Negative : public Filter {
public:
    Image Apply(const Image& image) const override;
};