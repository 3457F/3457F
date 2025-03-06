#define EIGEN_DONT_VECTORIZE
#define EIGEN_DISABLE_UNALIGNED_ARRAY_ASSERT
#include "Eigen/Dense"

class KalmanFilter {
public:
    Eigen::VectorXd X;  // state vector [x, y, theta]
    Eigen::MatrixXd P;  // covariance matrix
    Eigen::MatrixXd A;  // state transition model
    Eigen::MatrixXd H;  // observation model
    Eigen::MatrixXd Q;  // process noise
    Eigen::MatrixXd R;  // measurement noise
    Eigen::MatrixXd I;  // identity matrix
    
    KalmanFilter(double q, double r) {
        X = Eigen::VectorXd(3);  // [x, y, theta]
        P = Eigen::MatrixXd::Identity(3, 3);
        A = Eigen::MatrixXd::Identity(3, 3);  
        H = Eigen::MatrixXd::Identity(3, 3);
        Q = Eigen::MatrixXd::Identity(3, 3) * q; // model uncertainty
        R = Eigen::MatrixXd::Identity(3, 3) * r; // sensor variance
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
