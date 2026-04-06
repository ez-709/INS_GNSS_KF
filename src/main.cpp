#include "kalman_filter.hpp"
#include "imu_parser.hpp"
#include "gnss_parser.hpp"
#include "logger.hpp"
#include <time.h>

const long PERIOD_NS = 10'000'000L;

int main() {
    IMU_parser imu;
    GNSS_parser gnss;
    Logger logger;

    while (true) {
        struct timespec t_start;
        clock_gettime(CLOCK_MONOTONIC, &t_start);

        IMU_data imu_data = imu.read();
        GNSS_data gnss_data = gnss.read();
        logger.write(imu_data);
        logger.write(gnss_data);

        struct timespec t_end;
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