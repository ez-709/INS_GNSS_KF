#include "logger.hpp"
#include "bins_algoritm.hpp"

Logger::Logger() {
    imu_file.open("data/imu.csv");
    imu_file << "timestamp,wx,wy,wz,nx,ny,nz,mx,my,mz\n";

    gnss_file.open("data/gnss.csv");
    gnss_file << "timestamp,lat,lon,alt,VE,VN,Vh\n";

    bins_file.open("data/bins.csv");
    bins_file << "timestamp,lat,lon,alt,VE,VN,Vh\n";

    kf_file.open("data/kf.csv");
    kf_file << "timestamp,"
               "x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12,x13,"
               "s1,s2,s3,s4,s5,s6,s7,s8,s9,s10,s11,s12,s13\n";
}

void Logger::write(IMU_data data) {
    imu_file << data.timestamp << ","
             << data.wx << "," << data.wy << "," << data.wz << ","
             << data.nx << "," << data.ny << "," << data.nz << ","
             << data.mx << "," << data.my << "," << data.mz << "\n";
}

void Logger::write(GNSS_data data) {
    gnss_file << data.timestamp << ","
              << data.lat << "," << data.lon << "," << data.alt << ","
              << data.VE  << "," << data.VN  << "," << data.Vh  << "\n";
}

void Logger::write(const BINS_algoritm& bins) {
    bins_file << bins.getTimestamp() << ","
              << bins.getLat() << "," << bins.getLon() << "," << bins.getAlt() << ","
              << bins.getVE()  << "," << bins.getVN()  << "," << bins.getVh()  << "\n";
}

void Logger::write(VectorXd x, VectorXd s, double timestamp) {
    kf_file << timestamp << ",";
    for (int i = 0; i < x.size(); i++) kf_file << x(i) << ",";
    for (int i = 0; i < s.size(); i++) {
        kf_file << s(i);
        if (i != s.size() - 1) kf_file << ",";
    }
    kf_file << "\n";
}