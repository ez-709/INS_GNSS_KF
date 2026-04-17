#include "logger.hpp"

Logger::Logger() {
    imu_file.open("data/imu.csv");
    imu_file << "timestamp,wx,wy,wz,nx,ny,nz,mx,my,mz\n";

    gnss_file.open("data/gnss.csv");
    gnss_file << "timestamp,lon,lat,alt,VE,VN,Vh\n";

    estimation_file.open("data/estimation.csv");
    estimation_file << "x1,x2,x3,x4,alpha,beta,gamma,dOmX,dOmY,dOmZ,dnX,dnY,dnZ\n";
}

void Logger::write(IMU_data data) {
    imu_file << data.timestamp << ","
             << data.wx << "," << data.wy << "," << data.wz << ","
             << data.ax << "," << data.ay << "," << data.az << ","
             << data.mx << "," << data.my << "," << data.mz << "\n";
}

void Logger::write(GNSS_data data) {
    gnss_file << data.timestamp << ","
              << data.lon << "," << data.lot << "," << data.alt << ","
              << data.VE << "," << data.VN << "," << data.Vh << "\n";
}

void Logger::write(VectorXd data) {
    for (int i = 0; i < data.size(); i++) {
        estimation_file << data(i);
        if (i != data.size() - 1) estimation_file << ",";
    }
    estimation_file << "\n";
}