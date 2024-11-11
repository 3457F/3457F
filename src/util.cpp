#include "util.hpp"
#include <cmath>

bool within(double num, double expected_val, double range) {
    double diff = std::abs(num - expected_val);

    return diff < range;
}