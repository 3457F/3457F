#include "distance.hpp"
#include "consts.hpp"
#include "lemlib/pose.hpp"
#include "main.h"
#include "pros/distance.hpp"
#include "Eigen/Dense"
#include <cmath>
#include <iostream>
#include <numbers>
#include <vector>
#include <algorithm>
#include <format>

// Define field dimensions (field is 12" x 12", centered at (0,0))
const double LENGTH = 12.0;
const double WIDTH  = 12.0;

// Local (robot‐frame) sensor offsets (in inches)
// These represent the fixed mounting positions relative to the robot’s center.
SensorOffset leftOffset_local  = { -6,    -0.25 };
SensorOffset rightOffset_local = {  6,    -0.25 };
SensorOffset frontOffset_local = { -3.5,   5    };
SensorOffset backOffset_local  = {  5.75, -2.5  };

// Kalman Filter setup (assume the constructor takes Q and R parameters)
KalmanFilter kf(0.01, 0.1);

// Sensor objects
pros::Distance leftSensor(DIST_LEFT);
pros::Distance rightSensor(DIST_RIGHT);
pros::Distance frontSensor(DIST_FRONT);
pros::Distance backSensor(DIST_BACK);

// For median filtering
const int FILTER_SIZE = 5;
std::vector<double> leftReadings(FILTER_SIZE, 0);
std::vector<double> rightReadings(FILTER_SIZE, 0);
std::vector<double> frontReadings(FILTER_SIZE, 0);
std::vector<double> backReadings(FILTER_SIZE, 0);

lemlib::Pose calculatedPose = *(new lemlib::Pose(0, 0, 0));

// Helper: median filter
double getFilteredReading(std::vector<double>& readings, double newValue) {
    readings.push_back(newValue);
    if (readings.size() > FILTER_SIZE) {
        readings.erase(readings.begin()); // keep only the last N values
    }
    std::vector<double> sorted = readings;
    std::sort(sorted.begin(), sorted.end());
    return sorted[FILTER_SIZE / 2]; // return median value
}

// Helper: outlier detection
bool isOutlier(double prev, double current, double threshold = 3.0) {
    return fabs(prev - current) > threshold;
}

void updatePose() {
    // Read raw distances (convert mm to inches)
    double leftRaw  = leftSensor.get()  / 25.4;
    double rightRaw = rightSensor.get() / 25.4;
    double frontRaw = frontSensor.get() / 25.4;
    double backRaw  = backSensor.get()  / 25.4;

    // Apply median filtering
    double left  = getFilteredReading(leftReadings,  leftRaw);
    double right = getFilteredReading(rightReadings, rightRaw);
    double front = getFilteredReading(frontReadings, frontRaw);
    double back  = getFilteredReading(backReadings,  backRaw);

    // Read IMU angle in radians (robot’s current heading)
    double theta = imu.get_rotation() * (M_PI / 180.0);

    // Check for sudden outliers (using second-last reading as previous)
    if (isOutlier(leftReadings[FILTER_SIZE - 2], left))    left  = leftReadings[FILTER_SIZE - 2];
    if (isOutlier(rightReadings[FILTER_SIZE - 2], right))   right = rightReadings[FILTER_SIZE - 2];
    if (isOutlier(frontReadings[FILTER_SIZE - 2], front))   front = frontReadings[FILTER_SIZE - 2];
    if (isOutlier(backReadings[FILTER_SIZE - 2], back))     back  = backReadings[FILTER_SIZE - 2];

    // --- Compute rotated sensor offsets ---
    // These offsets are converted from the robot frame into the global frame.
    Eigen::Vector2d leftOffset_global(
        leftOffset_local.x * cos(theta) - leftOffset_local.y * sin(theta),
        leftOffset_local.x * sin(theta) + leftOffset_local.y * cos(theta)
    );
    Eigen::Vector2d rightOffset_global(
        rightOffset_local.x * cos(theta) - rightOffset_local.y * sin(theta),
        rightOffset_local.x * sin(theta) + rightOffset_local.y * cos(theta)
    );
    Eigen::Vector2d frontOffset_global(
        frontOffset_local.x * cos(theta) - frontOffset_local.y * sin(theta),
        frontOffset_local.x * sin(theta) + frontOffset_local.y * cos(theta)
    );
    Eigen::Vector2d backOffset_global(
        backOffset_local.x * cos(theta) - backOffset_local.y * sin(theta),
        backOffset_local.x * sin(theta) + backOffset_local.y * cos(theta)
    );

    // --- Compute candidate robot center estimates from each sensor pair ---
    // For the x-axis:
    // Left sensor: left wall is at x = -WIDTH/2.
    // Sensor global x = (-WIDTH/2) + left.
    // Robot center x = sensor_x - leftOffset_global.x.
    double x_from_left = ((-WIDTH/2) + left) - leftOffset_global.x();

    // Right sensor: right wall is at x = WIDTH/2.
    // Sensor global x = (WIDTH/2) - right.
    // Robot center x = sensor_x - rightOffset_global.x.
    double x_from_right = ((WIDTH/2) - right) - rightOffset_global.x();

    // For the y-axis:
    // Front sensor: front wall is at y = LENGTH/2.
    // Sensor global y = (LENGTH/2) - front.
    // Robot center y = sensor_y - frontOffset_global.y.
    double y_from_front = ((LENGTH/2) - front) - frontOffset_global.y();

    // Back sensor: back wall is at y = -LENGTH/2.
    // Sensor global y = (-LENGTH/2) + back.
    // Robot center y = sensor_y - backOffset_global.y.
    double y_from_back = ((-LENGTH/2) + back) - backOffset_global.y();

    // --- Choose (or blend) the estimates ---
    // Here we simply choose the sensor that sees the wall sooner (i.e. smaller reading).
    double x_center = (left < right) ? x_from_left : x_from_right;
    double y_center = (front < back) ? y_from_front : y_from_back;

    // get the raw measured pose estimate from the distance sensors
    Eigen::VectorXd Z(3);
    Z << x_center, y_center, theta;

    // feed this measured pose estimate to the kalman filter, and let the filter
    // weight this new (possibly uncertain) prediction against previous predictions
    kf.predict();
    kf.update(Z);

    // we now have a new weighted estimate for our robot's position!
    double filteredX     = kf.X(0);
    double filteredY     = kf.X(1);
    double filteredTheta = kf.X(2);

    // modifies odometry class's pose estimate to use these new values
    calculatedPose.x     = filteredX;
    calculatedPose.y     = filteredY;
    // imu is relatively reliable; not using kalman filter logic to update heading
    calculatedPose.theta = theta;

    // Print the results (requires C++20)
    std::cout << std::format(
        "x: {}, y: {}, theta: {}°, front: {}, left: {}, right: {}, back: {}",
        calculatedPose.x,
        calculatedPose.y,
        calculatedPose.theta * 180 / std::numbers::pi,
        front, left, right, back
    ) << std::endl;
}

void updateLoop() {
    while (true) {
        updatePose();
        pros::delay(100);
    }
}
