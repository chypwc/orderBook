#include <cmath>
#include "Vec3D.h"

float Vec3D::getLength() {
    return std::sqrt(x * x + y * y + z * z);
}