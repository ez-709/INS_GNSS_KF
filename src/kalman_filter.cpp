#include "kalman_filter.hpp"
#include <cmath>

KalmanFilter::KalmanFilter() {
    X_hat.setZero();

    P.setZero();
    P.diagonal() << 225.0, 225.0, 1.0, 1.0,
                    3e-4, 3e-4, 1.2e-3,
                    2.7e-8, 2.7e-8, 2.7e-8,
                    4e-2, 4e-2, 4e-2;

    double sq = (SIGMA_IMU * SIGMA_IMU) / T;
    Q = Eigen::Matrix<double, 6, 6>::Identity() * sq;

    double sr = (SIGMA_GNSS * SIGMA_GNSS) / T;
    R = Eigen::Matrix<double, 4, 4>::Identity() * sr;

    F.setZero();
    G.setZero();
    H.setZero();
    Phi.setZero();
    Gamma.setZero();
}

void KalmanFilter::predict(const BINS_algoritm& bins) {
    Eigen::Matrix3d C = bins.getC();
    Eigen::Vector3d n = bins.getNormal();
    double lat  = bins.getLat();
    double rho1 = bins.getRho1();
    double rho2 = bins.getRho2();

    double nE = n(0);
    double nH = n(2);

    F.setZero();
    F(0, 2) = 1.0;
    F(1, 3) = 1.0;
    F(2, 0) = -OMEGA0_SQ;
    F(3, 1) = -OMEGA0_SQ;
    F(2, 5) =  nH;
    F(3, 4) = -nH;
    F(3, 6) =  nE;
    F.block<3,3>(4, 7) = C;
    F.block<2,3>(2, 7) = C.topRows<2>();

    G.setZero();
    G.block<3,3>(4, 0) = C;
    G.block<2,3>(2, 3) = C.topRows<2>();

    Phi   = Eigen::Matrix<double,13,13>::Identity() + F * T;
    Gamma = G * T;

    Eigen::Matrix<double,13,13> S = Phi * P * Phi.transpose()
                                  + Gamma * Q * Gamma.transpose();
    X_hat = Phi * X_hat;
    P     = S;
}

void KalmanFilter::update(const GNSS_data& gnss, const BINS_algoritm& bins) {
    double lat  = bins.getLat();
    double rho1 = bins.getRho1();
    double rho2 = bins.getRho2();
    double Vh   = bins.getVh();

    double sr = (SIGMA_GNSS * SIGMA_GNSS) / T;
    R(0,0) = sr / (rho2 * rho2 * cos(lat) * cos(lat));
    R(1,1) = sr / (rho1 * rho1);
    R(2,2) = sr;
    R(3,3) = sr;

    H.setZero();
    H(0, 0) =  1.0 / (rho2 * cos(lat));
    H(1, 1) =  1.0 / rho1;
    H(2, 0) = -(Vh / rho2 + OMEGA_E * tan(lat));
    H(2, 2) =  1.0;
    H(3, 1) = -Vh / rho1;
    H(3, 3) =  1.0;

    Eigen::Matrix<double,4,1> Z;
    Z(0) = bins.getLon() - gnss.lon;
    Z(1) = bins.getLat() - gnss.lat;
    Z(2) = bins.getVE()  - gnss.VE;
    Z(3) = bins.getVN()  - gnss.VN;

    Eigen::Matrix<double,4,4> S_meas = H * P * H.transpose() + R;
    Eigen::Matrix<double,13,4> K = P * H.transpose() * S_meas.inverse();

    X_hat = X_hat + K * (Z - H * X_hat);

    Eigen::Matrix<double,13,13> I = Eigen::Matrix<double,13,13>::Identity();
    Eigen::Matrix<double,13,13> IKH = I - K * H;
    P = IKH * P * IKH.transpose() + K * R * K.transpose();
}