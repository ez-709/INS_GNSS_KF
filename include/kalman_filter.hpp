#pragma once
#include <Eigen/Dense>
#include "imu_parser.hpp"
#include "gnss_parser.hpp"
#include "config.hpp"

using namespace Eigen;

class KalmanFilter {
public:
    KalmanFilter();
    void predict(IMU_data imu);
    void update(GNSS_data gnss);

private:
    VectorXd X;
    MatrixXd P;
    MatrixXd F; 
    MatrixXd G; 
    MatrixXd H;  
    MatrixXd Q;   
    MatrixXd R;  
    MatrixXd Phi;  
    MatrixXd S;   
    MatrixXd C;  
};