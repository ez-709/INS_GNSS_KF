#pragma once
#include <Eigen/Dense>
#include "imu_parser.hpp"
#include "config.hpp"

class BINS_algoritm {
public:
    BINS_algoritm(double roll0, double pitch0, double yaw0);
    BINS_algoritm(double roll0, double pitch0, double yaw0,
                  double lat0, double lon0, double alt0);
    void update(IMU_data imu);
    void correct(Eigen::VectorXd dX);

    double getLat()       const { return lat; }
    double getLon()       const { return lon; }
    double getAlt()       const { return alt; }
    double getVE()        const { return VE; }
    double getVN()        const { return VN; }
    double getVh()        const { return Vh; }
    double getRho1()      const { return rho1; }
    double getRho2()      const { return rho2; }
    double getTimestamp() const { return timestamp; }
    Eigen::Matrix3d getC()      const { return C; }
    Eigen::Vector3d getNormal() const { return n_normal; }

private:
    void init_dcm(double roll0, double pitch0, double yaw0);
    void update_radius();
    void reorthogonalize();

    double timestamp = 0.0;
    double lat = LAT0;
    double lon = LON0;
    double alt = ALT0;
    double VE  = 0.0;
    double VN  = 0.0;
    double Vh  = 0.0;

    Eigen::Matrix3d C;
    Eigen::Vector3d n_normal;

    double rho1 = R;
    double rho2 = R;
    int    step = 0;
};