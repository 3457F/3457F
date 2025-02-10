#include "distance.hpp"
#include "lemlib/pose.hpp"
#include "pros/distance.hpp"
#include "pros/imu.hpp"
#include "Eigen/Dense"
#include <cmath>

// Define robot dimensions
const double LENGTH = 12.0;
const double WIDTH = 12.0;

// Distance sensor offsets (assumed mounted at edges)
SensorOffset leftOffset = { -WIDTH / 2, 0 };
SensorOffset rightOffset = { WIDTH / 2, 0 };
SensorOffset frontOffset = { 0, LENGTH / 2 };
SensorOffset backOffset = { 0, -LENGTH / 2 };

// Kalman Filter setup
KalmanFilter kf(0.01, 0.1);

// Sensor objects
pros::Distance leftSensor(1);
pros::Distance rightSensor(2);
pros::Distance frontSensor(3);
pros::Distance backSensor(4);
pros::Imu imu(5);

lemlib::Pose updatePose() {
    // Convert distance sensor readings to inches
    double left = leftSensor.get() / 25.4;
    double right = rightSensor.get() / 25.4;
    double front = frontSensor.get() / 25.4;
    double back = backSensor.get() / 25.4;

    // Read IMU angle in radians
    double theta = imu.get_rotation() * (M_PI / 180.0);

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

    return *(new lemlib::Pose(globalX, globalY, filteredTheta));
}
