#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>
#include "gnss_parser.hpp"

GNSS_data GNSS_parser::read(const struct timespec& t_start, const struct timespec& t_zero) {
    GNSS_data data;
    data.lon = 0;
    data.lot = 0;
    data.alt = 0;
    data.VE  = 0;
    data.VN  = 0;
    data.Vh  = 0;

    data.timestamp = (t_start.tv_sec  - t_zero.tv_sec)
                   + (t_start.tv_nsec - t_zero.tv_nsec) / 1e9;
    return data;
}