#pragma once
#include <Eigen/Dense>
#include "imu_parser.hpp"
#include "gnss_parser.hpp"
#include "bins_algoritm.hpp"
#include "config.hpp"

class KalmanFilter {
public:
    KalmanFilter();
    void predict(const BINS_algoritm& bins);
    void update(const GNSS_data& gnss, const BINS_algoritm& bins);

    Eigen::Matrix<double, 13, 1> getX() const { return X_hat; }
    Eigen::Matrix<double, 13, 13> getP() const { return P; }
    double getJ() const { return P.trace(); }

private:
    Eigen::Matrix<double, 13, 1> X_hat;
    Eigen::Matrix<double, 13, 13> P;

    Eigen::Matrix<double, 13,13> F;
    Eigen::Matrix<double, 13, 6> G;
    Eigen::Matrix<double, 4, 13> H;

    Eigen::Matrix<double, 13, 13> Phi;
    Eigen::Matrix<double, 13, 6> Gamma;

    Eigen::Matrix<double, 6, 6> Q;
    Eigen::Matrix<double, 4, 4> R;
};