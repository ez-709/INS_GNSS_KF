#pragma once
#include <Eigen/Dense>
#include "imu_parser.hpp"
#include "config.hpp"

class BINS_algoritm {
public:
    BINS_algoritm();
    void update(IMU_data imu);
    void correct(Eigen::VectorXd dX);

    double getLat() const { return lat; }
    double getLon() const { return lon; }
    double getAlt() const { return alt; }
    double getVE() const { return VE; }
    double getVN() const { return VN; }
    double getVh() const { return Vh; }
    Eigen::Matrix3d getC() const { return C; }
    Eigen::Vector3d getNormal() const { return n_normal; }

private:
    double lat = LAT0;
    double lon = LON0;
    double alt = ALT0;
    double VE = 0.0;
    double VN = 0.0;
    double Vh = 0.0;

    Eigen::Matrix3d C;
    Eigen::Vector3d n_normal; 

    double rho1 = R_EARTH;
    double rho2 = R_EARTH;

    int step = 0;

    void update_radii();
    void reorthogonalize();
};