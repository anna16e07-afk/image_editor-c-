#pragma once

#include "filter.h"
#include "../parser/parser.h"
#include <memory>

class FilterFactory {
public:
    static std::unique_ptr<Filter> Create(const FilterRequest& request);
};