#pragma once
#include <Eigen/Dense>

using namespace Eigen;

struct GNSS_data {
    double lon, lot, alt;
    double VE, VN, Vh;
    double timestamp;
};

class GNSS_parser {
    public:
        GNSS_data read();
    
    private:
        const char* uart_port = "/dev/ttyAMA0";
};