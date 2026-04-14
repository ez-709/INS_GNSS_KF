#pragma once
#include <fstream>
#include <Eigen/Dense>
#include "imu_parser.hpp"
#include "gnss_parser.hpp"
using namespace Eigen;

class Logger {
public:
    Logger();
    void write(IMU_data data);
    void write(GNSS_data data);
    void write(VectorXd data);
private:
    std::ofstream imu_file;
    std::ofstream gnss_file;
    std::ofstream estimation_file;
};