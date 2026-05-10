#pragma once
#include <cstdint>

struct IMU_data {
    double wx, wy, wz;
    double nx, ny, nz;
    double mx, my, mz;
    double timestamp = 0.0;
};

class IMU_parser {
public:
    IMU_parser();
    IMU_data read();
private:
    int fd_i2c = -1;

    static const int GYRO_I2C  = 0x68;
    static const int ACCEL_I2C = 0x53;
    static const int MAG_I2C   = 0x1E;

    static const uint8_t GYRO_REG       = 0x1D;
    static const uint8_t ACCEL_REG      = 0x32;
    static const uint8_t ACCEL_INIT_REG = 0x2D;
    static const uint8_t ACCEL_INIT_VAL = 0x08;
    static const uint8_t MAG_REG        = 0x03;
    static const uint8_t MAG_INIT_REG   = 0x02;
    static const uint8_t MAG_INIT_VAL   = 0x00;
};