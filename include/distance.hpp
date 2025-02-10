#pragma once
#include "lemlib/pose.hpp"
#include "pros/distance.hpp"
#include "kalman.hpp"

struct DistSensors {
    pros::Distance* left;
    pros::Distance* right;
    pros::Distance* front;
    pros::Distance* back;
};

struct SensorReadings {
    double left;
    double right;
    double front;
    double back;
};

struct SensorOffset {double x; double y;};

lemlib::Pose updatePose(double theta);