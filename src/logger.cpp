#include "logger.hpp"
#include <fstream>
#include <iostream>

void logger::write(IMU_data data) {
    std::ofstream f("data/imu.csv", std::ios::app);
    f << data.timestamp << "," << data.wx << "," << data.wy << "," << data.wz << ","
      << data.ax << "," << data.ay << "," << data.az << ","
      << data.mx << "," << data.my << "," << data.mz << std::endl;
    f.close();
}


void logger::write(GNSS_data data) {
    std::ofstream f("data/gnss.csv", std::ios::app);
    f << data.timestamp << "," << data.lon << "," << data.lot << "," << data.alt << ","
      << data.VE << "," << data.VN << "," << data.Vh << std::endl;
    f.close();
}

void logger::write(VectorXd data) {
    std::ofstream f("data/estimation.csv", std::ios::app);
    for (int i = 0; i < data.size(); i++) {
        f << data(i);
        if (i != data.size() - 1) f << ",";
    }
    f << std::endl;
    f.close();
}