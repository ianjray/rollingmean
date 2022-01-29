#pragma once


/// @brief Fuzzy floating point comparison.
/// @return True if parameters are equal after rounding to @c precision decimal places.
bool fcmp(double x, double y, int precision = 3);
