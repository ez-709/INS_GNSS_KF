#include "logger.hpp"
#include <fstream>
#include <iostream>

void Logger::write(IMU_data data) {
    std::ofstream f("data/imu.csv", std::ios::app);
    if (f.tellp() == 0) {
        f << "timestamp,wx,wy,wz,ax,ay,az,mx,my,mz\n";
    }
    f << data.timestamp << "," << data.wx << "," << data.wy << "," << data.wz << ","
      << data.ax << "," << data.ay << "," << data.az << ","
      << data.mx << "," << data.my << "," << data.mz << std::endl;
    f.close();
}

void Logger::write(GNSS_data data) {
    std::ofstream f("data/gnss.csv", std::ios::app);
    if (f.tellp() == 0) {
        f << "timestamp,lon,lat,alt,VE,VN,Vh\n";
    }
    f << data.timestamp << "," << data.lon << "," << data.lot << "," << data.alt << ","
      << data.VE << "," << data.VN << "," << data.Vh << std::endl;
    f.close();
}

void Logger::write(VectorXd data) {
    std::ofstream f("data/estimation.csv", std::ios::app);
    if (f.tellp() == 0) {
        f << "x1,x2,x3,x4,alpha,beta,gamma,dOmX,dOmY,dOmZ,dnX,dnY,dnZ\n";
    }
    for (int i = 0; i < data.size(); i++) {
        f << data(i);
        if (i != data.size() - 1) f << ",";
    }
    f << std::endl;
    f.close();
}