#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <Eigen/Dense>
#include "imu_parser.hpp"

IMU_parser::IMU_parser() {
    fd_i2c = open("/dev/i2c-1", O_RDWR);
    ioctl(fd_i2c, I2C_SLAVE, ACCEL_I2C);
    uint8_t buf[2] = {ACCEL_INIT_REG, ACCEL_INIT_VAL};
    write(fd_i2c, buf, 2);
    ioctl(fd_i2c, I2C_SLAVE, MAG_I2C);
    buf[0] = MAG_INIT_REG; buf[1] = MAG_INIT_VAL;
    write(fd_i2c, buf, 2);
}

IMU_data IMU_parser::read() {
    IMU_data data = {};
    uint8_t buf[6];

    ioctl(fd_i2c, I2C_SLAVE, ACCEL_I2C);
    write(fd_i2c, &ACCEL_REG, 1);
    ::read(fd_i2c, buf, 6);
    data.nx = int16_t(buf[1]<<8 | buf[0]) / 256.0 * 9.81;
    data.ny = int16_t(buf[3]<<8 | buf[2]) / 256.0 * 9.81;
    data.nz = int16_t(buf[5]<<8 | buf[4]) / 256.0 * 9.81;

    Eigen::Vector3d a(data.nx, data.ny, data.nz);
    Eigen::Vector3d d_cal(0.151, 0.509, 1.889);
    Eigen::Matrix3d M_cal;
    M_cal <<  0.9973, -0.1383, -0.1140,
             -0.0704,  0.9087,  0.0065,
             -0.1831,  0.0303,  0.9437;
    a = M_cal * (a - d_cal);
    data.nx = a(0);
    data.ny = a(1);
    data.nz = a(2);
    // ======================================

    ioctl(fd_i2c, I2C_SLAVE, GYRO_I2C);
    write(fd_i2c, &GYRO_REG, 1);
    ::read(fd_i2c, buf, 6);
    data.wx = int16_t(buf[0]<<8 | buf[1]) / 14.375 * 0.01745;
    data.wy = int16_t(buf[2]<<8 | buf[3]) / 14.375 * 0.01745;
    data.wz = int16_t(buf[4]<<8 | buf[5]) / 14.375 * 0.01745;

    ioctl(fd_i2c, I2C_SLAVE, MAG_I2C);
    write(fd_i2c, &MAG_REG, 1);
    ::read(fd_i2c, buf, 6);
    data.mx = int16_t(buf[0]<<8 | buf[1]) / 1090.0;
    data.mz = int16_t(buf[2]<<8 | buf[3]) / 1090.0;
    data.my = int16_t(buf[4]<<8 | buf[5]) / 1090.0;

    return data;
}