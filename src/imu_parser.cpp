#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <cstdint>
#include <time.h>
#include "imu_parser.hpp"

IMU_parser::IMU_parser() {
    int fd = open("/dev/i2c-1", O_RDWR);

    ioctl(fd, I2C_SLAVE, accel);
    uint8_t accel_init[2] = {0x2D, 0x08};
    write(fd, accel_init, 2);

    ioctl(fd, I2C_SLAVE, mag);
    uint8_t mag_init[2] = {0x02, 0x00};
    write(fd, mag_init, 2);

    close(fd);
}

IMU_data IMU_parser::read() {
    int fd = open("/dev/i2c-1", O_RDWR);
    IMU_data data;
    uint8_t reg;
    uint8_t buf[6];

    ioctl(fd, I2C_SLAVE, gyro);
    reg = 0x1D;
    write(fd, &reg, 1);
    ::read(fd, buf, 6);
    int16_t raw_wx = (buf[0] << 8) | buf[1];
    int16_t raw_wy = (buf[2] << 8) | buf[3];
    int16_t raw_wz = (buf[4] << 8) | buf[5];
    data.wx = raw_wx / 14.375 * 0.01745;
    data.wy = raw_wy / 14.375 * 0.01745;
    data.wz = raw_wz / 14.375 * 0.01745;

    ioctl(fd, I2C_SLAVE, accel);
    reg = 0x32;
    write(fd, &reg, 1);
    ::read(fd, buf, 6);
    int16_t raw_ax = (buf[1] << 8) | buf[0];
    int16_t raw_ay = (buf[3] << 8) | buf[2];
    int16_t raw_az = (buf[5] << 8) | buf[4];
    data.ax = raw_ax / 256.0 * 9.81;
    data.ay = raw_ay / 256.0 * 9.81;
    data.az = raw_az / 256.0 * 9.81;

    ioctl(fd, I2C_SLAVE, mag);
    reg = 0x03;
    write(fd, &reg, 1);
    ::read(fd, buf, 6);
    int16_t raw_mx = (buf[0] << 8) | buf[1];
    int16_t raw_mz = (buf[2] << 8) | buf[3];
    int16_t raw_my = (buf[4] << 8) | buf[5];
    data.mx = raw_mx / 1090.0;
    data.my = raw_my / 1090.0;
    data.mz = raw_mz / 1090.0;

    close(fd);
    return data;
}