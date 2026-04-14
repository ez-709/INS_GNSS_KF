#include "kalman_filter.hpp"
#include "config.hpp"
#include <cmath>

KalmanFilter::KalmanFilter() {
    X   = VectorXd::Zero(13);
    F   = MatrixXd::Zero(13, 13);
    P   = MatrixXd::Identity(13, 13) * 1000;
    G   = MatrixXd::Zero(13, 6);
    H   = MatrixXd::Zero(4, 13);
    Q   = MatrixXd::Identity(6, 6) * SIGMA_IMU;
    R   = MatrixXd::Identity(4, 4) * SIGMA_GNSS;
    Phi = MatrixXd::Identity(13, 13);
    S   = MatrixXd::Identity(13, 13);
    C   = MatrixXd::Identity(3, 3);
}

void KalmanFilter::predict(IMU_data imu) {
    MatrixXd Omega(3, 3);
    Omega <<       0, -imu.wz,  imu.wy,
             imu.wz,        0, -imu.wx,
            -imu.wy,   imu.wx,       0;
    C = C * (MatrixXd::Identity(3, 3) + Omega * T);

    Vector3d n = C * Vector3d(imu.ax, imu.ay, imu.az);
    double nE = n(0);
    double nH = n(2);

    F = MatrixXd::Zero(13, 13);
    F(0,2) = 1.0;
    F(1,3) = 1.0;
    F(2,0) = -OMEGA0_SQ;  F(2,5) =  nH;  F(2,7) = C(0,0);  F(2,8) = C(0,1);  F(2,9) = C(0,2);
    F(3,1) = -OMEGA0_SQ;  F(3,4) = -nH;  F(3,6) =  nE;     F(3,7) = C(1,0);  F(3,8) = C(1,1);  F(3,9) = C(1,2);
    F(4,7) = C(0,0);  F(4,8) = C(0,1);  F(4,9) = C(0,2);
    F(5,7) = C(1,0);  F(5,8) = C(1,1);  F(5,9) = C(1,2);
    F(6,7) = C(2,0);  F(6,8) = C(2,1);  F(6,9) = C(2,2);

    G = MatrixXd::Zero(13, 6);
    G(2,3) = C(0,0);  G(2,4) = C(0,1);  G(2,5) = C(0,2);
    G(3,3) = C(1,0);  G(3,4) = C(1,1);  G(3,5) = C(1,2);
    G(4,0) = C(0,0);  G(4,1) = C(0,1);  G(4,2) = C(0,2);
    G(5,0) = C(1,0);  G(5,1) = C(1,1);  G(5,2) = C(1,2);
    G(6,0) = C(2,0);  G(6,1) = C(2,1);  G(6,2) = C(2,2);

    Phi = MatrixXd::Identity(13, 13) + F * T;
    MatrixXd Gamma = G * T;
    S = Phi * P * Phi.transpose() + Gamma * Q * Gamma.transpose();
    P = S;
}

void KalmanFilter::update(GNSS_data gnss) {
    double phi  = gnss.lot * M_PI / 180.0;
    double h    = gnss.alt;
    double sphi = sin(phi);
    double rho1 = R_EARTH * (1 - E2) / pow(1 - E2 * sphi * sphi, 1.5) + h;
    double rho2 = R_EARTH / sqrt(1 - E2 * sphi * sphi) + h;

    double OmE = OMEGA_E * cos(phi);
    double OmH = OMEGA_E * sin(phi);

    H = MatrixXd::Zero(4, 13);
    H(0,0) =  1.0 / (rho2 * cos(phi));
    H(1,1) =  1.0 / rho1;
    H(2,0) = -(gnss.Vh / rho2 + OmE * tan(phi));
    H(2,1) = -OmH;
    H(2,2) =  1.0;
    H(3,0) = -gnss.Vh / rho1;
    H(3,3) =  1.0;

    VectorXd Z(4);
    Z << X(0) - gnss.lon,
         X(1) - gnss.lot,
         X(2) - gnss.VE,
         X(3) - gnss.VN;

    MatrixXd K = S * H.transpose() * (H * S * H.transpose() + R).inverse();
    X = Phi * X + K * (Z - H * Phi * X);
    P = (MatrixXd::Identity(13, 13) - K * H) * S;
}