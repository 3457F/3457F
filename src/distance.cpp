#include "distance.hpp"
#include "consts.hpp"
#include "lemlib/pose.hpp"
#include "main.h"
#include "pros/distance.hpp"
#include "Eigen/Dense"
#include <cmath>

// Define field dimensions
const double LENGTH = 12.0;
const double WIDTH = 12.0;

// Distance sensor offsets (assumed mounted at edges)
SensorOffset leftOffset = {0, 0};
SensorOffset rightOffset = {0, 0};
SensorOffset frontOffset = {0, 0};
SensorOffset backOffset = {0, 0};

// Kalman Filter setup
KalmanFilter kf(0.01, 0.1);

// Sensor objects
pros::Distance leftSensor(DIST_LEFT);
pros::Distance rightSensor(DIST_RIGHT);
pros::Distance frontSensor(DIST_FRONT);
pros::Distance backSensor(DIST_BACK);

const int FILTER_SIZE = 5;
std::vector<double> leftReadings(FILTER_SIZE, 0);
std::vector<double> rightReadings(FILTER_SIZE, 0);
std::vector<double> frontReadings(FILTER_SIZE, 0);
std::vector<double> backReadings(FILTER_SIZE, 0);

lemlib::Pose calculatedPose = *(new lemlib::Pose(0, 0, 0));

double getFilteredReading(std::vector<double>& readings, double newValue) {
    readings.push_back(newValue);
    if (readings.size() > FILTER_SIZE) {
        readings.erase(readings.begin()); // Keep only the last N values
    }
    std::vector<double> sorted = readings;
    std::sort(sorted.begin(), sorted.end());
    return sorted[FILTER_SIZE / 2]; // Return median value
}

bool isOutlier(double prev, double current, double threshold = 3.0) {
    return fabs(prev - current) > threshold;
}

void updatePose() {
    // Read raw distances
    double leftRaw = leftSensor.get() / 25.4;
    double rightRaw = rightSensor.get() / 25.4;
    double frontRaw = frontSensor.get() / 25.4;
    double backRaw = backSensor.get() / 25.4;

    // Apply median filtering
    double left = getFilteredReading(leftReadings, leftRaw);
    double right = getFilteredReading(rightReadings, rightRaw);
    double front = getFilteredReading(frontReadings, frontRaw);
    double back = getFilteredReading(backReadings, backRaw);

    // Read IMU angle in radians
    double theta = imu.get_rotation() * (M_PI / 180.0);

    // Ignore sensors with sudden outliers (set them to last good value)
    if (isOutlier(leftReadings[FILTER_SIZE - 2], left)) left = leftReadings[FILTER_SIZE - 2];
    if (isOutlier(rightReadings[FILTER_SIZE - 2], right)) right = rightReadings[FILTER_SIZE - 2];
    if (isOutlier(frontReadings[FILTER_SIZE - 2], front)) front = frontReadings[FILTER_SIZE - 2];
    if (isOutlier(backReadings[FILTER_SIZE - 2], back)) back = backReadings[FILTER_SIZE - 2];

    // Compute x and y using midpoint of distances
    double x_raw = (left - right + WIDTH) / 2.0;
    double y_raw = (back - front + LENGTH) / 2.0;

    // Adjust for sensor offsets (without rotating them)
    double x_adjusted = x_raw + (leftOffset.x - rightOffset.x) / 2.0;
    double y_adjusted = y_raw + (backOffset.y - frontOffset.y) / 2.0;

    // Create measurement vector
    Eigen::VectorXd Z(3);
    Z << x_adjusted, y_adjusted, theta;

    // Kalman filter update
    kf.predict();
    kf.update(Z);

    // Retrieve filtered pose
    double filteredX = kf.X(0);
    double filteredY = kf.X(1);
    double filteredTheta = kf.X(2);

    // Transform to global coordinate frame
    Eigen::Matrix2d rotationMatrix;
    rotationMatrix << cos(filteredTheta), -sin(filteredTheta),
                                sin(filteredTheta),  cos(filteredTheta);

    Eigen::Vector2d globalPosition = rotationMatrix * Eigen::Vector2d(filteredX, filteredY);

    // Extract final transformed pose
    double globalX = globalPosition(0);
    double globalY = globalPosition(1);

    calculatedPose.x = globalX;
    calculatedPose.y = globalY;
    calculatedPose.theta = theta;
}

void updateLoop() {
    bool a = true;
    while(true) {
        updatePose();
        pros::delay(34);

        a = !a;
        if (a) { 
            controller.set_text(
                0, 
                0, 
                std::format(
                    "x: {}, y: {}, theta: {}", 
                    calculatedPose.x, 
                    calculatedPose.y, 
                    calculatedPose.theta
                    )
                );
            }
    }
}