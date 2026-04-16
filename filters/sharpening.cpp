#include "sharpening.h"

constexpr int Centre = 5;

MatrixFilter::Matrix Sharpening::GetMatrix() const {
    return {{0, -1, 0}, {-1, Centre, -1}, {0, -1, 0}};
}