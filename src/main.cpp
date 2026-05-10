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
    GNSS_parser gnss;
    IMU_parser  imu;
    Logger      logger;

    double nx=0, ny=0, nz=0, mx=0, my=0, mz=0;
    for (int i = 0; i < 200; i++) {
        IMU_data d = imu.read();
        nx+=d.nx; ny+=d.ny; nz+=d.nz;
        mx+=d.mx; my+=d.my; mz+=d.mz;
        struct timespec ts = {0, 10'000'000L};
        nanosleep(&ts, nullptr);
    }
    nx/=200; ny/=200; nz/=200;
    mx/=200; my/=200; mz/=200;

    double roll  = atan2(ny, nz);
    double pitch = atan2(-nx, sqrt(ny*ny + nz*nz));
    double mx_c  = mx*cos(pitch) + mz*sin(pitch);
    double my_c  = mx*sin(roll)*sin(pitch) + my*cos(roll) - mz*sin(roll)*cos(pitch);
    double yaw   = atan2(-my_c, mx_c);

    GNSS_data gnss_data;
    while (!gnss_data.valid)
        gnss_data = gnss.read();
    printf("GNSS fix = True\n");

    BINS_algoritm bins(roll, pitch, yaw);
    KalmanFilter  kf;

    struct timespec t_zero;
    clock_gettime(CLOCK_MONOTONIC, &t_zero);

    while (true) {
        struct timespec t_start;
        clock_gettime(CLOCK_MONOTONIC, &t_start);

        IMU_data  imu_data  = imu.read();
        GNSS_data gnss_data = gnss.read();

        double ts = (t_start.tv_sec  - t_zero.tv_sec)
                  + (t_start.tv_nsec - t_zero.tv_nsec) / 1e9;
        imu_data.timestamp  = ts;
        gnss_data.timestamp = ts;

        bins.update(imu_data);
        kf.predict(bins);

        if (gnss_data.valid && gnss_data.fresh) {
            kf.update(gnss_data, bins);
            bins.correct(kf.getX());
            gnss_data.fresh = false;
        }

        logger.write(imu_data);
        logger.write(gnss_data);
        logger.write(bins);

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