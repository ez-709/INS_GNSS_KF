#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "imu_parser.hpp"

IMU_data IMU_parser::read() {
    IMU_data data;
    return data;
}