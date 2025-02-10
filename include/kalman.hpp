#define EIGEN_DONT_VECTORIZE
#define EIGEN_DISABLE_UNALIGNED_ARRAY_ASSERT
#include "Eigen/Dense"
#include "pros/distance.hpp"
#include "pros/imu.hpp"

// im ngl i got no clue if this works 
// this is what chat gutpa gave me

class KalmanFilter {
public:
    Eigen::VectorXd X;  // State vector [x, y, theta]
    Eigen::MatrixXd P;  // Covariance matrix
    Eigen::MatrixXd A;  // State transition model
    Eigen::MatrixXd H;  // Observation model
    Eigen::MatrixXd Q;  // Process noise
    Eigen::MatrixXd R;  // Measurement noise
    Eigen::MatrixXd I;  // Identity matrix
    
    KalmanFilter(double q, double r) {
        X = Eigen::VectorXd(3);  // [x, y, theta]
        P = Eigen::MatrixXd::Identity(3, 3);
        A = Eigen::MatrixXd::Identity(3, 3);  
        H = Eigen::MatrixXd::Identity(3, 3);
        Q = Eigen::MatrixXd::Identity(3, 3) * q;
        R = Eigen::MatrixXd::Identity(3, 3) * r;
        I = Eigen::MatrixXd::Identity(3, 3);

        X << 0, 0, 0;  // Initial state
    }

    void predict() {
        X = A * X;  
        P = A * P * A.transpose() + Q;
    }

    void update(Eigen::VectorXd Z) {
        Eigen::VectorXd Y = Z - (H * X);
        Eigen::MatrixXd S = H * P * H.transpose() + R;
        Eigen::MatrixXd K = P * H.transpose() * S.inverse();

        X = X + (K * Y);
        P = (I - K * H) * P;
    }
};
