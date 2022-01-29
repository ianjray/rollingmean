#include "fcmp.hpp"

#include <cmath>

bool fcmp(double x, double y, int precision)
{
    auto scale = pow(10, precision);
    return round(x * scale) == round(y * scale);
}
