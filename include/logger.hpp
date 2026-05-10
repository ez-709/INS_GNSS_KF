#pragma once
#include <fstream>
#include <Eigen/Dense>
#include "imu_parser.hpp"
#include "gnss_parser.hpp"
using namespace Eigen;

class BINS_algoritm;

class Logger {
public:
    Logger();
    void write(IMU_data data);
    void write(GNSS_data data);
    void write(const BINS_algoritm& bins);
    void write(VectorXd x, VectorXd s, double timestamp);
private:
    std::ofstream imu_file;
    std::ofstream gnss_file;
    std::ofstream bins_file;
    std::ofstream kf_file;
};