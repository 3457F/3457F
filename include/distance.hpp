#pragma once
#include "lemlib/pose.hpp"
#include "pros/distance.hpp"
#include "kalman.hpp"

struct SensorOffset {double x; double y;};

void updatePose(double theta);
void updateLoop();

extern lemlib::Pose calculatedPose;