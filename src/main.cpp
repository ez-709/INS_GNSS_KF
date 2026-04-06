#include "kalman_filter.hpp"
#include "imu_parser.hpp"
#include "gnss_parser.hpp"
#include "logger.hpp"
#include <unistd.h>

int main() {
    IMU_parser imu;
    GNSS_parser gnss;
    Logger logger;

    while (true) {
        IMU_data imu_data = imu.read();
        GNSS_data gnss_data = gnss.read();
        logger.write(imu_data);
        logger.write(gnss_data);
        usleep(10000);
    }

    return 0;
}