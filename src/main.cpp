#include <cmath>
#include <cstdio>
#include <time.h>
#include "gnss_parser.hpp"
#include "imu_parser.hpp"
#include "bins_algoritm.hpp"
#include "kalman_filter.hpp"
#include "logger.hpp"
#include "config.hpp"

int main() {
    IMU_parser imu;
    Logger     logger;

    printf("Recording IMU for calibration... Ctrl+C to stop\n");
    printf("Place on each face for 6-10 seconds\n");

    struct timespec t_zero;
    clock_gettime(CLOCK_MONOTONIC, &t_zero);

    while (true) {
        struct timespec t_start;
        clock_gettime(CLOCK_MONOTONIC, &t_start);

        IMU_data imu_data = imu.read();
        double ts = (t_start.tv_sec  - t_zero.tv_sec)
                  + (t_start.tv_nsec - t_zero.tv_nsec) / 1e9;
        imu_data.timestamp = ts;

        logger.write(imu_data);

        struct timespec t_end;
        clock_gettime(CLOCK_MONOTONIC, &t_end);
        long elapsed = (t_end.tv_sec  - t_start.tv_sec)  * 1'000'000'000L
                     + (t_end.tv_nsec - t_start.tv_nsec);
        long sleep = PERIOD_NS - elapsed;
        if (sleep > 0) {
            struct timespec s = {0, sleep};
            nanosleep(&s, nullptr);
        }
    }
}