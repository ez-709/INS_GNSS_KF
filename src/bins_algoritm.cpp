#include <cmath>
#include "bins_algoritm.hpp"
#include "imu_parser.hpp"
#include "config.hpp"

BINS_algoritm::BINS_algoritm(double roll0, double pitch0, double yaw0) {
    C << cos(pitch0)*cos(yaw0),  sin(roll0)*sin(pitch0)*cos(yaw0) - cos(roll0)*sin(yaw0),  cos(roll0)*sin(pitch0)*cos(yaw0) + sin(roll0)*sin(yaw0),
         cos(pitch0)*sin(yaw0),  sin(roll0)*sin(pitch0)*sin(yaw0) + cos(roll0)*cos(yaw0),  cos(roll0)*sin(pitch0)*sin(yaw0) - sin(roll0)*cos(yaw0),
        -sin(pitch0),            sin(roll0)*cos(pitch0),                                    cos(roll0)*cos(pitch0);

    n_normal = Eigen::Vector3d::Zero();
}

void BINS_algoritm::update(IMU_data imu) {
    Eigen::Matrix3d omega;
    omega << 0,        -imu.wz,  imu.wy,
             imu.wz,   0,       -imu.wx,
            -imu.wy,   imu.wx,   0;

    C = C * (Eigen::Matrix3d::Identity() + omega * T);

    Eigen::Vector3d n_body(imu.nx, imu.ny, imu.nz);
    n_normal = C * n_body;
    n_normal(2) -= G;

    VE += n_normal(0) * T;
    VN += n_normal(1) * T;
    Vh += n_normal(2) * T;

    lat += VN / rho1 * T;
    lon += VE / (rho2 * cos(lat)) * T;
    alt += Vh * T;

    step++;
    if (step % 100 == 0) reorthogonalize();
    update_radius();
}

void BINS_algoritm::correct(Eigen::VectorXd dX) {

}

void BINS_algoritm::reorthogonalize() {
    Eigen::JacobiSVD<Eigen::Matrix3d> svd(C, Eigen::ComputeFullU | Eigen::ComputeFullV);
    C = svd.matrixU() * svd.matrixV().transpose();
}

void BINS_algoritm::update_radius() {
    double temp = 1.0 - E2 * sin(lat) * sin(lat);
    rho1 = R * (1.0 - E2) / (temp * sqrt(temp));
    rho2 = R / sqrt(temp);
}