#pragma once

#include "matrix_filter.h"

class Sharpening : public MatrixFilter {
protected:
    Matrix GetMatrix() const override;
};