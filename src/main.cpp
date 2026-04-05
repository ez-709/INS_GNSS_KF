#include "kalman_filter.hpp"
#include "imu_parser.hpp"
#include "gnss_parser.hpp"
#include "logger.hpp"

int main() {
    IMU_parser imu;
    GNSS_parser gnss;
    Logger logger;

    IMU_data imu_data = imu.read();
    GNSS_data gnss_data = gnss.read();
    logger.write(imu_data);
    logger.write(gnss_data);
    return 0;
}