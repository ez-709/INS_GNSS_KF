#pragma once
#include <time.h>

struct GNSS_data {
    double lon, lot, alt;
    double VE, VN, Vh;
    double timestamp;
};

class GNSS_parser {
public:
    GNSS_data read(const struct timespec& t_start, const struct timespec& t_zero);

private:
    const char* uart_port = "/dev/ttyAMA0";
};