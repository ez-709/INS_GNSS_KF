#pragma once
#include <time.h>

struct IMU_data {
    double wx, wy, wz;
    double ax, ay, az;
    double mx, my, mz;
    double timestamp;
};

class IMU_parser {
    public:
        IMU_data read(const struct timespec& t_start, const struct timespec& t_zero);

    private:
        int gyro  = 0x68;
        int accel = 0x53;
        int mag   = 0x1E;
};