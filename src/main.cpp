#include <time.h>
#include "gnss_parser.hpp"
#include "imu_parser.hpp"
#include "kalman_filter.hpp"
#include "logger.hpp"
#include "config.hpp"

int main() {
    GNSS_parser gnss;
    IMU_parser  imu;
    Logger      logger;
    KalmanFilter kf;

    struct timespec t_zero;
    clock_gettime(CLOCK_MONOTONIC, &t_zero);

    struct timespec t_start;
    struct timespec t_end;

    while (true) {
        clock_gettime(CLOCK_MONOTONIC, &t_start);

        IMU_data  imu_data  = imu.read();
        GNSS_data gnss_data = gnss.read();

        double timestamp = (t_start.tv_sec  - t_zero.tv_sec)
                         + (t_start.tv_nsec - t_zero.tv_nsec) / 1e9;
        imu_data.timestamp  = timestamp;
        gnss_data.timestamp = timestamp;

        kf.predict(imu_data);
        kf.update(gnss_data);

        logger.write(imu_data);
        logger.write(gnss_data);

        clock_gettime(CLOCK_MONOTONIC, &t_end);
        long elapsed_ns = (t_end.tv_sec  - t_start.tv_sec)  * 1'000'000'000L
                        + (t_end.tv_nsec - t_start.tv_nsec);
        long sleep_ns = PERIOD_NS - elapsed_ns;
        if (sleep_ns > 0) {
            struct timespec ts = {0, sleep_ns};
            nanosleep(&ts, nullptr);
        }
    }
    return 0;
}