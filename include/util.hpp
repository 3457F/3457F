#pragma once

#include "api.h"
#include "lemlib/api.hpp"
#include "arm.hpp"

struct FetchInfo {
    lemlib::PID* pid;
    Arm* arm;
    float* target;
    pros::Rotation* encoder;
};

struct SetInfo {
    lemlib::PID* pid;
    float error;
    std::int32_t curr_angle;
    float* target;
};